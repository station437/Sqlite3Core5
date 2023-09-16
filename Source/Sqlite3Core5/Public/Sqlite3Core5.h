// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Sqlite3Core5DataTypes.h"
#include "Modules/ModuleManager.h"

class USqlite3Core5Settings;

class FSqlite3Core5Module : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FSqlite3Core5Module& Get()
    {
        return FModuleManager::LoadModuleChecked<FSqlite3Core5Module>("Sqlite3Core5");
    }

	USqlite3Core5Settings* GetSettings() const;

	bool IsLibraryLoaded();
    void RunCustomQueryGeneric(FString Query, TArray<FSqliteGenericRow>& GenericResult, FString DbFullname = "");
    FString TestSql();

private:
	/** Handle to the test dll we will load */
    void* Sqlite3LibraryHandle = nullptr;

	static int Callback(void* result, int argc, char** argv, char** azColName);

protected:
    USqlite3Core5Settings* ModuleSettings;

};
