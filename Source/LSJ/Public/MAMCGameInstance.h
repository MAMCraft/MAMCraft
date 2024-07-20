// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "MAMCGameInstance.generated.h"

USTRUCT()
struct  FEnemyData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "STAT")
	int level;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "STAT")
	int attackDamage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "STAT")
	int moveSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "STAT")
	int maxHp;
};

/**
 * 
 */

//인벤토리 필수 데이터
USTRUCT()
struct FItemStruct
{
	GENERATED_USTRUCT_BODY()

	/*UPROPERTY()
	FText UseActionText;
	UPROPERTY()
	UTexture2D* Thumnail;
	UPROPERTY()
	TArray<FName> Tags;*/
	UPROPERTY()
	int32 category;
	UPROPERTY()
	int32 itemID;
	UPROPERTY()
	int32 count;
	//default properties
	FItemStruct()
	{
		category = -1;
		itemID = -1;
		//UseActionText = FText::FromString(TEXT(""));
		//Thumnail=nullptr;
		//Tags.Empty();
	}
};


//										//could use a class instead of struct
//FORCEINLINE FArchive &operator <<(FArchive &Ar, FItemStruct& TheStruct )
//{
//	Ar << TheStruct.VictoryVibe;
//	Ar << TheStruct.VictoryTransform;
//	Ar << TheStruct.MeshPath;
//	Ar << TheStruct.StartsDisabled;
//		
//	return Ar;
//}


UCLASS()
class LSJ_API UMAMCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMAMCGameInstance();

	virtual void Init() override;
	UFUNCTION()
	FEnemyData GetStatData(FName name);
	TArray<FItemStruct> itemStruct;
	void Save(TArray<AItem*>& inputItems);
	void Load(TArray<AItem*>& outputItems);
	virtual void Shutdown() override;
	//void SaveCharacterInfo();
	//void LoadCharacterInfo();
	void SetLife(int amount);
	int GetLife();
	void SetPlayerHp(int setHp);
	int GetPlayerHp();
	void SetHPCooldownRemainTime(float remainTime);
	float GetHPCooldownRemainTime();
	UFUNCTION()
	void SetPlayerHpPosionEnable();
	FTimerHandle instanceHPCooldownTimerHandle;
private:
	UPROPERTY()
	class UDataTable* myStats;
	int playerCurrentHp;
	float instanceHPCooldownRemainTime=0.f;
	int life = 1;
};

