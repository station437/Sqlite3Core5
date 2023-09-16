// Fill out your copyright notice in the Description page of Project Settings.


#include "Sqlite3Core5Functions.h"
#include "Modules/ModuleManager.h"
#include "UObject/TextProperty.h"
#include "Async/Async.h"

USqlite3Core5Settings *USqlite3Core5Functions::GetSqlite3Core5Settings()
{
    return FSqlite3Core5Module::Get().GetSettings();
}

bool USqlite3Core5Functions::IsLibraryLoaded()
{
    FModuleManager &ModManager = FModuleManager::Get();

    return ModManager.IsModuleLoaded("Sqlite3Core5")
        && ModManager.GetModulePtr<FSqlite3Core5Module>("Sqlite3Core5")->IsLibraryLoaded();
}

FString USqlite3Core5Functions::TestSql()
{
    FModuleManager &ModManager = FModuleManager::Get();
    FSqlite3Core5Module* Module = ModManager.GetModulePtr<FSqlite3Core5Module>("Sqlite3Core5");
    if (Module == nullptr)
        return "Module is not loaded";

    return Module->TestSql();
}

void USqlite3Core5Functions::RunCustomQueryGeneric(FString DbFullname, FString Query, TArray<FSqliteGenericRow>& GenericResult)
{
    FModuleManager &ModManager  = FModuleManager::Get();
    FSqlite3Core5Module *Module = ModManager.GetModulePtr<FSqlite3Core5Module>("Sqlite3Core5");
    if (Module)
        Module->RunCustomQueryGeneric(Query, GenericResult, DbFullname);
}

void USqlite3Core5Functions::RunCustomQuery(
    FString DbFullname,
    FString Query,
    TSubclassOf<UObject> RowClass,
    TArray<UObject*>& OutObjects)
{
    TArray<FSqliteGenericRow> GenericResult;
	RunCustomQueryGeneric(DbFullname, Query, GenericResult);
    OutObjects.Reset();
 
    for (FSqliteGenericRow Row: GenericResult)
    {       
        UObject *RowPtr = NewObject<UObject>(GetTransientPackage(), RowClass);      

        for (int i = 0; i < Row.Columns.Num(); ++i)
        {
            FillQueryResultColumn(RowPtr, Row.Columns[i], Row.Values[i]);
        }       
        
        OutObjects.Add(RowPtr);
    }
}

void USqlite3Core5Functions::RunInsertQuery(
    FString DbFullname,
    FSqliteInsertQuery InsertQuery,
    TArray<FSqliteGenericRow>& GenericResult)
{
	RunCustomQueryGeneric(DbFullname, FormInsertQuery(InsertQuery), GenericResult);
}

void USqlite3Core5Functions::RunUpdateQuery(
    FString DbFullname,
    FSqliteUpdateQuery UpdateQuery,
    TArray<FSqliteGenericRow>& GenericResult)
{
	RunCustomQueryGeneric(DbFullname, FormUpdateQuery(UpdateQuery), GenericResult);
}

void USqlite3Core5Functions::RunDeleteQuery(
	FString DbFullname,
	FSqliteDeleteQuery DeleteQuery,
	TArray<FSqliteGenericRow>& GenericResult)
{
	RunCustomQueryGeneric(DbFullname, FormDeleteQuery(DeleteQuery), GenericResult);
}

void USqlite3Core5Functions::AsyncRunCustomQueryGeneric(FString DbFullname, FString Query, const FQueryResultGenericDelegate& Completed)
{
	AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [Completed, Query, DbFullname]()
    {
		TArray<FSqliteGenericRow> GenericResult = TArray<FSqliteGenericRow>();
		RunCustomQueryGeneric(DbFullname, Query, GenericResult);

		AsyncTask(ENamedThreads::GameThread, [Completed, GenericResult]()
        {
            Completed.ExecuteIfBound(GenericResult);
        });
	});
}

void USqlite3Core5Functions::AsyncRunCustomQuery(
    FString DbFullname,
    FString Query,
    TSubclassOf<UObject> RowClass,
    const FQueryResultDelegate& Completed)
{
	AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [Completed, Query, DbFullname, RowClass]() {

        TArray<FSqliteGenericRow> GenericResult = TArray<FSqliteGenericRow>();
		RunCustomQueryGeneric(DbFullname, Query, GenericResult);
		TArray<UObject*> Result;

        for (FSqliteGenericRow Row : GenericResult)
		{
			UObject* RowPtr = NewObject<UObject>(GetTransientPackage(), RowClass);

			for (int i = 0; i < Row.Columns.Num(); ++i)
			{
				FillQueryResultColumn(RowPtr, Row.Columns[i], Row.Values[i]);
			}

			Result.Add(RowPtr);
		}

		AsyncTask(ENamedThreads::GameThread, [Completed, Result]() {
			Completed.ExecuteIfBound(Result);
		});
	});
}

void USqlite3Core5Functions::AsyncRunSelectQuery(FString DbFullname, FSqliteSelectQuery SelectQuery, TSubclassOf<UObject> RowClass, const FQueryResultDelegate& Completed)
{
	AsyncRunCustomQuery(DbFullname, FormSelectQuery(SelectQuery), RowClass, Completed);
}

void USqlite3Core5Functions::AsyncRunInsertQuery(FString DbFullname, FSqliteInsertQuery InsertQuery, const FQueryResultGenericDelegate& Completed)
{
	AsyncRunCustomQueryGeneric(DbFullname, FormInsertQuery(InsertQuery), Completed);
}

void USqlite3Core5Functions::AsyncRunUpdateQuery(FString DbFullname, FSqliteUpdateQuery UpdateQuery, const FQueryResultGenericDelegate& Completed)
{
	AsyncRunCustomQueryGeneric(DbFullname, FormUpdateQuery(UpdateQuery), Completed);
}

void USqlite3Core5Functions::AsyncRunDeleteQuery(FString DbFullname, FSqliteDeleteQuery DeleteQuery, const FQueryResultGenericDelegate& Completed)
{
	AsyncRunCustomQueryGeneric(DbFullname, FormDeleteQuery(DeleteQuery), Completed);
}

void USqlite3Core5Functions::FillQueryResultColumn(UObject *Object, FName Column, FString Value)
{
    FProperty *Property = Object->GetClass()->FindPropertyByName(Column);

    if (Property)
    {
        void *ValuePtr = Property->ContainerPtrToValuePtr<void>(Object);

        if (CastField<FNumericProperty>(Property))
        {
            if (FIntProperty *IntProperty = CastField<FIntProperty>(Property))
            {
                IntProperty->SetPropertyValue(ValuePtr, FCString::Atoi(*Value));
                return;
            }

            if (FInt64Property* Int64Property = CastField<FInt64Property>(Property))
			{
				Int64Property->SetPropertyValue(ValuePtr, FCString::Atoi64(*Value));
				return;
			}

            if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
            {
                FloatProperty->SetPropertyValue(ValuePtr, FCString::Atof(*Value));
                return;
            }

            if (FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(Property))
            {
                DoubleProperty->SetPropertyValue(ValuePtr, FCString::Atod(*Value));
                return;
            }
        }
                
        if (FStrProperty* StrProperty = CastField<FStrProperty>(Property))
        {
            StrProperty->SetPropertyValue(ValuePtr, Value);
            return;
        }

        
        if (FTextProperty* TextProperty = CastField<FTextProperty>(Property))
        {
            TextProperty->SetPropertyValue(ValuePtr, FText::FromString(Value));
            return;
        }

        if (FNameProperty* NameProperty = CastField<FNameProperty>(Property))
        {
            NameProperty->SetPropertyValue(ValuePtr, FName(Value));
            return;
        }

        if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
        {
            BoolProperty->SetPropertyValue(ValuePtr, FCString::Atoi(*Value) == 1 ? true : false);
            return;
        }
    }
}

FString USqlite3Core5Functions::FormSelectQuery(const FSqliteSelectQuery& SelectQuery)
{
	FString Result = "SELECT";
	Result.Append(SelectQuery.Columns).Append(" FROM ").Append(SelectQuery.Table);

    if (!SelectQuery.JoinType.IsEmpty())
	{
		Result.Append(SelectQuery.JoinType)
			.Append(" JOIN ")
			.Append(SelectQuery.JoinTable)
			.Append(" ON ")
			.Append(SelectQuery.JoinCondition);
	}

	if(!SelectQuery.WhereLeftValue.IsEmpty())
    {
		Result.Append(" WHERE ")
			.Append(SelectQuery.WhereLeftValue)
			.Append(" ")
			.Append(SelectQuery.WhereOperand)
			.Append(" ")
			.Append(SelectQuery.WhereRightValue);
    }

    if (!SelectQuery.OrderBy.IsEmpty())
	{
		Result.Append(" ORDER BY ").Append(SelectQuery.OrderBy);
	}

    if (!SelectQuery.Limit.IsEmpty())
	{
		Result.Append(" LIMIT ").Append(SelectQuery.Limit);
		if (!SelectQuery.Offset.IsEmpty())
		{
			Result.Append(" OFFSET ").Append(SelectQuery.Offset);
		}
	}

    if (!SelectQuery.GroupBy.IsEmpty())
	{
		Result.Append(" GROUP BY ").Append(SelectQuery.GroupBy);
	}

    if (!SelectQuery.Having.IsEmpty())
	{
		Result.Append(" HAVING ").Append(SelectQuery.Having);
	}

    Result.Append(";");
    return Result;
}

FString USqlite3Core5Functions::FormInsertQuery(const FSqliteInsertQuery& InsertQuery)
{
	FString Result = "INSERT INTO ";
	Result.Append(InsertQuery.Table)
		.Append(" (")
		.Append(InsertQuery.Columns)
		.Append(") VALUES (")
		.Append(InsertQuery.Values)
		.Append(");");
	return Result;
}

FString USqlite3Core5Functions::FormUpdateQuery(const FSqliteUpdateQuery& UpdateQuery)
{
	FString Result = "UPDATE ";
	Result.Append(UpdateQuery.Table);

	Result.Append(" SET ")
		.Append(UpdateQuery.SetLeftValue)
		.Append(" = ")
		.Append(UpdateQuery.SetRightValue);

	if (!UpdateQuery.WhereLeftValue.IsEmpty())
	{
		Result.Append(" WHERE ")
			.Append(UpdateQuery.WhereLeftValue)
			.Append(" ")
			.Append(UpdateQuery.WhereOperand)
			.Append(" ")
			.Append(UpdateQuery.WhereRightValue);
	}

	if (!UpdateQuery.OrderBy.IsEmpty())
	{
		Result.Append(" ORDER BY ").Append(UpdateQuery.OrderBy);
	}

	if (!UpdateQuery.Limit.IsEmpty())
	{
		Result.Append(" LIMIT ").Append(UpdateQuery.Limit);
		if (!UpdateQuery.Offset.IsEmpty())
		{
			Result.Append(" OFFSET ").Append(UpdateQuery.Offset);
		}
	}

	Result.Append(";");
	return Result;
}

FString USqlite3Core5Functions::FormDeleteQuery(const FSqliteDeleteQuery& DeleteQuery)
{
	FString Result = "DELETE FROM ";
	Result.Append(DeleteQuery.Table);

	if (!DeleteQuery.WhereLeftValue.IsEmpty())
	{
		Result.Append(" WHERE ")
			.Append(DeleteQuery.WhereLeftValue)
			.Append(" ")
			.Append(DeleteQuery.WhereOperand)
			.Append(" ")
			.Append(DeleteQuery.WhereRightValue);
	}

	if (!DeleteQuery.OrderBy.IsEmpty())
	{
		Result.Append(" ORDER BY ").Append(DeleteQuery.OrderBy);
	}

	if (!DeleteQuery.Limit.IsEmpty())
	{
		Result.Append(" LIMIT ").Append(DeleteQuery.Limit);
		if (!DeleteQuery.Offset.IsEmpty())
		{
			Result.Append(" OFFSET ").Append(DeleteQuery.Offset);
		}
	}

	Result.Append(";");
	return Result;
}
