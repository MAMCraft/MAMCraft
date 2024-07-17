// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSword.h"
#include "Components/WidgetComponent.h"

void AItemSword::BeginPlay()
{
	Super::BeginPlay();
}

AItemSword::AItemSword()
{
	//아이템설명
	UseActionText = FText::FromString("Sword");
	//텍스트 지정
	FString Path = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/Sword/T_Sword_Steel_Icon_inventory.T_Sword_Steel_Icon_inventory'");
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	Thumnail = Texture;
	Tags.Add(FName("ItemSword"));
	category = (int)EItemCategroy::sword;
}
