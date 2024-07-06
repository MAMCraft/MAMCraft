// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "LSJEngine.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API ULSJEngine : public UEngine
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct  FEnemyStatData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STAT")
	int32 level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STAT")
	int32 attackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STAT")
	int32 moveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "STAT")
	int32 maxHp;
};