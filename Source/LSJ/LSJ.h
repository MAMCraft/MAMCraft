// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LSJ, Log, All);

#define LSJLOG_CALLINFO (FString(__FUNCTION__)+TEXT("(")+FString::FromInt(__LINE__)+TEXT(")"))
#define LSJLOG_S(Verbosity) UE_LOG(LSJ,Verbosity,TEXT("%s"),*LOGMAMC_CALLINFO)
#define LSJLOG(Verbosity,Format,...) UE_LOG(LSJ,Verbosity,TEXT("%s %s"),*LOGMAMC_CALLINFO,*FString::Printf(Format,##__VA_ARGS__))
