// Fill out your copyright notice in the Description page of Project Settings.


#include "MAMCGameInstance.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
// MyGameInstance.cpp
UMAMCGameInstance::UMAMCGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DATA(TEXT("/Script/Engine.DataTable'/Game/LSJ/Data/StatTable.StatTable'"));

	myStats = DATA.Object;

}
//캐릭터 생성전에 시작
void UMAMCGameInstance::Init()
{
	Super::Init();

}
FEnemyData UMAMCGameInstance::GetStatData(FName name)
{
	//return myStats->FindRow<FEnemyData>(*FString::FromInt(level), TEXT(""));
	return *myStats->FindRow<FEnemyData>(name, "",false);
}
//아이템 저장
void UMAMCGameInstance::Save(TArray<AItem*>& inputItems)
{
	itemStruct.Empty();
	for (int index = 0; index < inputItems.Num(); index++)
	{
		FItemStruct itemTemp;
		/*	itemTemp.UseActionText = inputItems[index]->UseActionText;
			itemTemp.Thumnail = inputItems[index]->Thumnail;
			itemTemp.Tags = inputItems[index]->Tags;*/
		itemTemp.category = inputItems[index]->category;
		itemTemp.itemID = inputItems[index]->itemID;
		itemTemp.count = inputItems[index]->count;
		itemStruct.Add(itemTemp);
	}
}
//아이템 로드
void UMAMCGameInstance::Load(TArray<AItem*>& outputItems)
{
	for (int index = 0; index < itemStruct.Num(); index++)
	{
		
		//outputItems[index]->UseActionText = itemStruct[index].UseActionText;
		//outputItems[index]->Thumnail = itemStruct[index].Thumnail;
		//outputItems[index]->Tags = itemStruct[index].Tags;
		outputItems[index]->category = itemStruct[index].category;
		outputItems[index]->itemID = itemStruct[index].itemID;
		outputItems[index]->count = itemStruct[index].count;
	}
	
}

void UMAMCGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UMAMCGameInstance::SetLife(int amount)
{
	life = amount;
}

int UMAMCGameInstance::GetLife()
{
	return life;
}

void UMAMCGameInstance::SetPlayerHp(int setHp)
{
	playerCurrentHp = setHp;
}

int UMAMCGameInstance::GetPlayerHp()
{
	return playerCurrentHp;
}

void UMAMCGameInstance::SetHPCooldownRemainTime(float remainTime)
{
	instanceHPCooldownRemainTime = remainTime;
	GetTimerManager().SetTimer(instanceHPCooldownTimerHandle,this, &UMAMCGameInstance::SetPlayerHpPosionEnable, instanceHPCooldownRemainTime, false);
}

float UMAMCGameInstance::GetHPCooldownRemainTime()
{
	return instanceHPCooldownRemainTime;
}

void UMAMCGameInstance::SetPlayerHpPosionEnable()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player->bIsHPCooldownActive = false;
	instanceHPCooldownRemainTime = 0;
}
