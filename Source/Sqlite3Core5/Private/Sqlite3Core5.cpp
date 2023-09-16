// Copyright Epic Games, Inc. All Rights Reserved.

#include "Sqlite3Core5.h"
#include "Sqlite3Core5Settings.h"
//#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "UObject/Package.h"
#include "Misc/Paths.h"

#include "sqlite3.h"

#define LOCTEXT_NAMESPACE "FSqlite3Core5Module"

void FSqlite3Core5Module::StartupModule()
{
	ModuleSettings = NewObject<USqlite3Core5Settings>(GetTransientPackage(), "Sqlite3Core5Settings", RF_Standalone);
    ModuleSettings->AddToRoot();

    // Register settings
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->RegisterSettings(
            "Project", "Plugins", "Sqlite3Core5", LOCTEXT("RuntimeSettingsName", "Sqlite3 Core5"), LOCTEXT("RuntimeSettingsDescription", "Configure Sqlite3Core5 plugin settings"), ModuleSettings);
    }

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("Sqlite3Core5")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;

#if PLATFORM_WINDOWS
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Sqlite3Native/lib/win64/sqlite3.dll"));
#elif PLATFORM_MAC
    //LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/Sqlite3Core5Library/Mac/Release/libExampleLibrary.dylib"));
#elif PLATFORM_LINUX
    LibraryPath = FPaths::Combine( *BaseDir, TEXT( "Sqlite3Native" ), TEXT( "lib" ), TEXT( "linux" ), TEXT( "sqlite3.so" ) );
#endif // PLATFORM_WINDOWS

#if UE_SERVER || UE_EDITOR
        Sqlite3LibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
#else
    if (!GetSettings()->bServerSideOnly)
        Sqlite3LibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
#endif
	
	if (Sqlite3LibraryHandle)
    {
        //FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibrarySuccess", "Sqlite 3 library loaded!"));
        UE_LOG(LogTemp, Display, TEXT("Sqlite3 library is loaded."));
    }

}

void FSqlite3Core5Module::ShutdownModule()
{
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->UnregisterSettings("Project", "Plugins", "Sqlite3Core5");
    }

    if (!GExitPurge)
    {
        ModuleSettings->RemoveFromRoot();
    }
    else
    {
        ModuleSettings = nullptr;
    }

	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
    if (Sqlite3LibraryHandle)
    {
        FPlatformProcess::FreeDllHandle(Sqlite3LibraryHandle);
        Sqlite3LibraryHandle = nullptr;
    }
}

USqlite3Core5Settings* FSqlite3Core5Module::GetSettings() const
{
    check(ModuleSettings) return ModuleSettings;
}

bool FSqlite3Core5Module::IsLibraryLoaded()
{
    return Sqlite3LibraryHandle ? true : false;
}

void FSqlite3Core5Module::RunCustomQueryGeneric(FString Query, TArray<FSqliteGenericRow>& GenericResult, FString DbFullname)
{
    if (!IsLibraryLoaded())
        return;

    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    FString BaseRelativePath = "";
    FString DbFinalName;

    if (DbFullname.IsEmpty())
        DbFinalName = GetSettings()->DefaultDbFilename;
    else
        DbFinalName = DbFullname;

    if (!DbFinalName.StartsWith("/") && !DbFinalName.Contains(":"))
    {
#if PLATFORM_WINDOWS
        DbFinalName.ReplaceInline(TEXT("/"), TEXT("\\"));
#elif PLATFORM_LINUX
        DbFinalName.ReplaceInline(TEXT("\\"), TEXT("/"));
#endif

#if UE_EDITOR
        BaseRelativePath = FPaths::Combine(
            IPluginManager::Get().FindPlugin("Sqlite3Core5")->GetBaseDir(), 
            TEXT("Databases")
            );
#else
        BaseRelativePath = FPaths::Combine(TEXT(".."), TEXT(".."), TEXT(".."));
#endif
        DbFinalName = FPaths::Combine(BaseRelativePath, DbFinalName);
    }

    rc = sqlite3_open_v2(
        TCHAR_TO_UTF8(*DbFinalName),
        &db,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
        NULL);
    if (rc)
    {
        UE_LOG(LogTemp, Error, TEXT("sqlite3 error: %s"), UTF8_TO_TCHAR(sqlite3_errmsg(db)));
        sqlite3_close_v2(db);
    }

    GenericResult.Reset();

    rc = sqlite3_exec(db, TCHAR_TO_UTF8(*Query), Callback, &GenericResult, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        UE_LOG(LogTemp, Error, TEXT("sql error: %s"), UTF8_TO_TCHAR(zErrMsg));
        sqlite3_free(zErrMsg);
    }
    sqlite3_close_v2(db);
}

int FSqlite3Core5Module::Callback(void* result, int argc, char** argv, char** azColName)
{
    TArray<FSqliteGenericRow>* Result = StaticCast<TArray<FSqliteGenericRow>*>(result);
    if (!Result)
        return 1;

    FName col;
    FString val;
    TArray<FName> RowColumns;
    TArray<FString> RowValues;

    for (int i = 0; i < argc; ++i)
    {
        col = UTF8_TO_TCHAR(azColName [i]);
        val = UTF8_TO_TCHAR(argv [i]);
        RowColumns.Add(col);
        RowValues.Add(val);
    }

    FSqliteGenericRow Row;
    Row.Columns = RowColumns;
    Row.Values  = RowValues;
    Result->Add(Row);
    return 0;
}

FString FSqlite3Core5Module::TestSql()
{
    if (!IsLibraryLoaded())
        return "library is not loaded";

    TArray<FSqliteGenericRow> Result;
    RunCustomQueryGeneric("select \'pas\' || \'sed\' as result;", Result);
    if (Result.Num() == 1)
        return Result [0].Values [0];
    return "failed";
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSqlite3Core5Module, Sqlite3Core5)
