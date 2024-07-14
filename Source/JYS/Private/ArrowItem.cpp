// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowItem.h"

// Sets default values
AArrowItem::AArrowItem()
{
	//아이템설명
	UseActionText = FText::FromString("Arrow");
	//텍스트 지정
	FString Path = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/arrow.arrow'");
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	Thumnail = Texture;
	//아이템명 으로 탐색 for문
	ItemDisplayName = FText::FromString("ItemArrow");
	Tags.Add(FName("ItemArrow"));
	category = (int)EItemCategroy::arrow;
}
void AArrowItem::BeginPlay()
{
	Super::BeginPlay();
}