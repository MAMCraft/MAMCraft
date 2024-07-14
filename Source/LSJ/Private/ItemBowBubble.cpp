// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBowBubble.h"
#include "Components/WidgetComponent.h"


void AItemBowBubble::BeginPlay()
{
	Super::BeginPlay();
}

AItemBowBubble::AItemBowBubble()

{	//�����ۼ���
	UseActionText = FText::FromString("BowBubble");
	//�ؽ�Ʈ ����
	FString Path = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/Bow/T_BubbleBow_Spooky2_Icon_Inventory.T_BubbleBow_Spooky2_Icon_Inventory'");
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	Thumnail = Texture;
	//�����۸� ���� Ž�� for��
	ItemDisplayName = FText::FromString("ItemBowBubble");
	Tags.Add(FName("ItemBowBubble"));
	category = (int)EItemCategroy::bow;
}