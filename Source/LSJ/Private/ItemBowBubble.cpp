// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBowBubble.h"
#include "Components/WidgetComponent.h"


void AItemBowBubble::BeginPlay()
{
	Super::BeginPlay();
}

AItemBowBubble::AItemBowBubble()

{	//아이템설명
	UseActionText = FText::FromString("BowBubble");
	//텍스트 지정
	FString Path = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/Bow/T_BubbleBow_Spooky2_Icon_Inventory.T_BubbleBow_Spooky2_Icon_Inventory'");
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	Thumnail = Texture;
	//아이템명 으로 탐색 for문
	ItemDisplayName = FText::FromString("ItemBowBubble");
	Tags.Add(FName("ItemBowBubble"));
	category = (int)EItemCategroy::bow;
}
