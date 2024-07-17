// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBowBasic.h"
#include "Components/WidgetComponent.h"

void AItemBowBasic::BeginPlay()
{
	Super::BeginPlay();
}

AItemBowBasic::AItemBowBasic()
{
	//�����ۼ���
	UseActionText = FText::FromString("BowBasic");
	//�ؽ�Ʈ ����
	FString Path = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/Bow/T_Bow_Icon_inventory.T_Bow_Icon_inventory'");
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	Thumnail = Texture;
	itemID = 1;
	Tags.Add(FName("ItemBowBasic"));
	category = (int)EItemCategroy::bow;
}
