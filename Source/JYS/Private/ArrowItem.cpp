// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowItem.h"

// Sets default values
AArrowItem::AArrowItem()
{
	//�����ۼ���
	UseActionText = FText::FromString("Arrow");
	//�ؽ�Ʈ ����
	FString Path = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/arrow.arrow'");
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));
	Thumnail = Texture;
	//�����۸� ���� Ž�� for��
	ItemDisplayName = FText::FromString("ItemArrow");
	Tags.Add(FName("ItemArrow"));
	category = (int)EItemCategroy::arrow;
}
void AArrowItem::BeginPlay()
{
	Super::BeginPlay();
}