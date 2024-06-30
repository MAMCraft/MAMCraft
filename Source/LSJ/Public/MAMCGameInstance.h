// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MAMCGameInstance.generated.h"

USTRUCT()
struct  FEnemyData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 attackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 moveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 maxHp;
};

/**
 * 
 */
UCLASS()
class LSJ_API UMAMCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMAMCGameInstance();

	virtual void Init() override;

	FEnemyData* GetStatData(FName name);

private:
	UPROPERTY()
	class UDataTable* myStats;

};

