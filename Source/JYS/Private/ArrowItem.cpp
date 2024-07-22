// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowItem.h"

// Sets default values
AArrowItem::AArrowItem()
{

	Tags.Add(FName("ItemArrow"));
	category = (int)EItemCategroy::arrow;
	itemID = 3;
	//텍스트 지정
	FString Path = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/arrow.arrow'");
	Thumnail = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
}
void AArrowItem::BeginPlay()
{
	Super::BeginPlay();
}