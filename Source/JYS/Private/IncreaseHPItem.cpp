// Fill out your copyright notice in the Description page of Project Settings.


#include "IncreaseHPItem.h"
#include "Components/StaticMeshComponent.h"
#include <PlayerCharacter.h>
#include <Kismet/GameplayStatics.h>
// Sets default values
AIncreaseHPItem::AIncreaseHPItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//아이템설명
	UseActionText = FText::FromString("IncreaseHPItem");
	//텍스트 지정
	FString Path = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/Hp/potion_empty.potion_empty'");
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	Thumnail = Texture;
	Tags.Add(FName("IncreaseHPItem"));
	category = (int)EItemCategroy::posion;
}

void AIncreaseHPItem::BeginPlay()
{
	Super::BeginPlay();
}