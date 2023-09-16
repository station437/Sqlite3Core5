// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sqlite3Core5Settings.generated.h"

/**
 * 
 */
//UCLASS(config = Plugins, DefaultConfig, meta=(DisplayName="Sqlite3Core5"))
UCLASS(config = Engine, DefaultConfig)
//class SQLITE3CORE5_API USqlite3Core5Settings: public UDeveloperSettings
class SQLITE3CORE5_API USqlite3Core5Settings: public UObject
{
    GENERATED_UCLASS_BODY()

public:

    UPROPERTY(Config, EditAnywhere, Category = "Main")
    FString DefaultDbFilename = "default.db";

    UPROPERTY(Config, EditAnywhere, Category = "Main")
    bool bServerSideOnly = true;
};
