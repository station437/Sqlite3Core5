#pragma once

#include "Engine/EngineBaseTypes.h"

#include "Sqlite3Core5DataTypes.generated.h"

USTRUCT(BlueprintType)
struct SQLITE3CORE5_API FSqliteGenericRow
{
    GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main")
    TArray<FName> Columns;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main")
    TArray<FString> Values;

    FSqliteGenericRow() {};
};

USTRUCT(BlueprintType)
struct SQLITE3CORE5_API FSqliteSelectQuery
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Columns = "*";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Table = "table";

	/* LEFT or INNER */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString JoinType = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString JoinTable = "";

	/* something like table1.common_column = table2.common_column */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString JoinCondition = "";

	/* column1 in condition: column1 = 10 */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString WhereLeftValue = "";

	/* = in condition: column1 = 10 */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString WhereOperand = "";

	/* 10 in condition: column1 = 10 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString WhereRightValue = "";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString OrderBy = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Limit = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Offset = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString GroupBy = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Having = "";

	FSqliteSelectQuery(){};
};

USTRUCT(BlueprintType)
struct SQLITE3CORE5_API FSqliteInsertQuery
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Table = "";

	/* column1, column2, ...*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Columns = "";

	/* value1, value2, ...*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Values = "";

	FSqliteInsertQuery(){};
};

USTRUCT(BlueprintType)
struct SQLITE3CORE5_API FSqliteUpdateQuery
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Table = "";

	/* column1 in condition: column1 = 10 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString SetLeftValue = "";

	/* 10 in condition: column1 = 10 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString SetRightValue = "";

	/* column1 in condition: column1 = 10 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString WhereLeftValue = "";

	/* = in condition: column1 = 10 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString WhereOperand = "";

	/* 10 in condition: column1 = 10 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString WhereRightValue = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString OrderBy = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Limit = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Offset = "";

	FSqliteUpdateQuery(){};
};

USTRUCT(BlueprintType)
struct SQLITE3CORE5_API FSqliteDeleteQuery
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Table = "";

	/* column1 in condition: column1 = 10 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString WhereLeftValue = "";

	/* = in condition: column1 = 10 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString WhereOperand = "";

	/* 10 in condition: column1 = 10 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString WhereRightValue = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString OrderBy = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Limit = "";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main")
	FString Offset = "";

	FSqliteDeleteQuery(){};
};