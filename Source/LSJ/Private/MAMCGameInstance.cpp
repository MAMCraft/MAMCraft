// Fill out your copyright notice in the Description page of Project Settings.


#include "MAMCGameInstance.h"

// MyGameInstance.cpp
UMAMCGameInstance::UMAMCGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DATA(TEXT("/Script/Engine.DataTable'/Game/LSJ/Data/StatTable.StatTable'"));

	myStats = DATA.Object;
}

void UMAMCGameInstance::Init()
{
	Super::Init();

}
FEnemyData UMAMCGameInstance::GetStatData(FName name)
{
	//return myStats->FindRow<FEnemyData>(*FString::FromInt(level), TEXT(""));
	return *myStats->FindRow<FEnemyData>(name, "",false);
}