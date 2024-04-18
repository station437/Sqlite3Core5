// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Templates/SubclassOf.h"

#include "Sqlite3Core5.h"
#include "sqlite3.h"

#include "Sqlite3Core5Functions.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE_OneParam(FQueryResultGenericDelegate, const TArray<FSqliteGenericRow>&, Rows);
DECLARE_DYNAMIC_DELEGATE_OneParam(FQueryResultDelegate, const TArray<UObject*>&, Rows);

UCLASS()
class SQLITE3CORE5_API USqlite3Core5Functions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    UFUNCTION(BlueprintPure, Category = "Sqlite3Core5|Common")
    static USqlite3Core5Settings* GetSqlite3Core5Settings();

    UFUNCTION(BlueprintCallable, Category = "Sqlite3Core5|Common")
    static UPARAM(DisplayName = "Loaded") bool IsLibraryLoaded();

    UFUNCTION(BlueprintCallable, Category = "Sqlite3Core5|Common")
    static UPARAM(DisplayName = "TestResult") FString TestSql();

    UFUNCTION(BlueprintCallable, Category = "Sqlite3Core5|Common")
    static void RunCustomQueryGeneric(
		FString DbFullname,
        FString Query,
        UPARAM(DisplayName = "SqliteGenericRows") TArray<FSqliteGenericRow>& GenericResult
    );

    UFUNCTION(BlueprintCallable,
              Category = "Sqlite3Core5|Common",
              meta     = (DeterminesOutputType = "RowClass", DynamicOutputParam = "OutObjects"))
    static void RunCustomQuery(
		FString DbFullname,
        FString Query,
        TSubclassOf<UObject> RowClass,
        UPARAM(DisplayName = "Rows") TArray<UObject*>& OutObjects
    );

	UFUNCTION(BlueprintCallable, Category = "Sqlite3Core5|Common")
	static void RunSelectQuery(
		FString DbFullname,
		FSqliteSelectQuery SelectQuery,
		UPARAM(DisplayName = "SqliteGenericRows") TArray<FSqliteGenericRow>& GenericResult);


    UFUNCTION(BlueprintCallable, Category = "Sqlite3Core5|Common")
	static void RunInsertQuery(
		FString DbFullname,
		FSqliteInsertQuery InsertQuery,
		UPARAM(DisplayName = "SqliteGenericRows") TArray<FSqliteGenericRow>& GenericResult);

    UFUNCTION(BlueprintCallable,
		Category = "Sqlite3Core5|Common")
	static void RunUpdateQuery(
		FString DbFullname,
		FSqliteUpdateQuery UpdateQuery,
		UPARAM(DisplayName = "SqliteGenericRows") TArray<FSqliteGenericRow>& GenericResult);

    UFUNCTION(BlueprintCallable,
		Category = "Sqlite3Core5|Common")
	static void RunDeleteQuery(
		FString DbFullname,
		FSqliteDeleteQuery DeleteQuery,
		UPARAM(DisplayName = "SqliteGenericRows") TArray<FSqliteGenericRow>& GenericResult);

    UFUNCTION(BlueprintCallable, Category = "Sqlite3Core5|Async")
	static void AsyncRunCustomQueryGeneric(
		FString DbFullname,
        FString Query,
        const FQueryResultGenericDelegate& Completed
    );

    UFUNCTION(BlueprintCallable, Category = "Sqlite3Core5|Async")
	static void AsyncRunCustomQuery(
		FString DbFullname,
		FString Query,
		TSubclassOf<UObject> RowClass,
		const FQueryResultDelegate& Completed
    );

    UFUNCTION(BlueprintCallable, Category = "Sqlite3Core5|Async")
	static void AsyncRunSelectQuery(
		FString DbFullname,
		FSqliteSelectQuery SelectQuery,
		TSubclassOf<UObject> RowClass,
		const FQueryResultDelegate& Completed);

    UFUNCTION(BlueprintCallable, Category = "Sqlite3Core5|Async")
	static void AsyncRunInsertQuery(
		FString DbFullname,
		FSqliteInsertQuery InsertQuery,
		const FQueryResultGenericDelegate& Completed);

    UFUNCTION(BlueprintCallable, Category = "Sqlite3Core5|Async")
	static void AsyncRunUpdateQuery(
		FString DbFullname,
		FSqliteUpdateQuery UpdateQuery,
		const FQueryResultGenericDelegate& Completed);

	UFUNCTION(BlueprintCallable, Category = "Sqlite3Core5|Async")
	static void AsyncRunDeleteQuery(
		FString DbFullname,
		FSqliteDeleteQuery DeleteQuery,
		const FQueryResultGenericDelegate& Completed);

private:
    UFUNCTION()
    static void FillQueryResultColumn(UObject* QueryResultRow, FName Column, FString Value);

    UFUNCTION()
	static FString FormSelectQuery(const FSqliteSelectQuery& SelectQuery);

    UFUNCTION()
	static FString FormInsertQuery(const FSqliteInsertQuery& InsertQuery);
    
    UFUNCTION()
	static FString FormUpdateQuery(const FSqliteUpdateQuery& UpdateQuery);

    UFUNCTION()
	static FString FormDeleteQuery(const FSqliteDeleteQuery& DeleteQuery);
};
