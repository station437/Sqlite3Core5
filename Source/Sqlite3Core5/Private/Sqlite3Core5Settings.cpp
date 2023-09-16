// Fill out your copyright notice in the Description page of Project Settings.

#include "Sqlite3Core5Settings.h"

USqlite3Core5Settings::USqlite3Core5Settings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    DefaultDbFilename = "default.db";
    bServerSideOnly   = true;
}

