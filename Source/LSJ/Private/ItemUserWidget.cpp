// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUserWidget.h"

void UItemUserWidget::NativeConstruct()
{
	//FString tArrowPath = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/arrow.arrow'");
	//FString tSwordPath = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/Sword/T_Sword_Steel_Icon_inventory.T_Sword_Steel_Icon_inventory'");
	//FString tBowBasicPath = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/Bow/T_Bow_Icon_inventory.T_Bow_Icon_inventory'");
	//FString tBowBubblePath = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/Bow/T_BubbleBow_Spooky2_Icon_Inventory.T_BubbleBow_Spooky2_Icon_Inventory'");
	//tArrow = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *tArrowPath));
	//tSword = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *tSwordPath));
	//tBowBasic = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *tBowBasicPath));
	//tBowBubble = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *tBowBubblePath));
}

void UItemUserWidget::BindItemImage(FText& name, UTexture2D* texture)
{
	ItemImage->SetBrushFromTexture(texture);
	//if (name.EqualTo(FText::FromString("ItemSword")))
}
