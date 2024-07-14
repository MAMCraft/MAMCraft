// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UE_LOG(LogTemp, Log, TEXT("UpdateCanTick, %f %s"), GetWorld()->GetTimerManager().GetTimerRemaining(player->HPCooldownTimerHandle), GetWorld()->GetTimerManager().IsTimerActive(player->HPCooldownTimerHandle)?TEXT("null") : TEXT("Yes"));
	if (GetWorld()->GetTimerManager().IsTimerActive(player->HPCooldownTimerHandle))
	{
		PosionCoolTimeBar->SetVisibility(ESlateVisibility::Visible);
		UpdatePosionCoolTimeUpdate();
	}
}

void UInventoryWidget::UpdatePosionCoolTimeUpdate()
{
	PosionCoolTimeBar->SetPercent(GetWorld()->GetTimerManager().GetTimerRemaining(player->HPCooldownTimerHandle)/60.0f);
}

void UInventoryWidget::AddToViewport()
{
	Super::AddToViewport();
	UpdateInventory();
}

void UInventoryWidget::UpdateInventory()
{
	Items = inventoryComponent->Items;
	if (Items.IsEmpty())
	{
		ItemQuantity1->SetText(FText::AsNumber(0));
		//FSlateColor InvertedForeground = FLinearColor(0, 0, 0);
		//ItemIcon1->SetBrushTintColor(InvertedForeground);
		return;
	}
	//count�� ����
	if (Items[0]->count <= 0)
	{
		return;
	}
	//�κ��丮�� �־��ֱ�
	for (auto* item : Items)
	{
		switch (item->category)
		{
		case (int)EItemCategroy::posion:
			ItemQuantity1->SetText(FText::AsNumber(Items[0]->count));
			ItemIconHp->SetBrushFromTexture(Items[0]->Thumnail);
		case (int)EItemCategroy::sword:
			ItemQuantitySword->SetText(FText::AsNumber(Items[1]->count));
			ItemIconSword->SetBrushFromTexture(Items[1]->Thumnail);
		case (int)EItemCategroy::bow:
			ItemQuantityBow->SetText(FText::AsNumber(Items[2]->count));
			ItemIconBow->SetBrushFromTexture(Items[2]->Thumnail);
		default:
			break;
		}
	}

	//FSlateColor InvertedForeground = FLinearColor(1, 1, 1);
	//ItemIcon1->SetBrushTintColor(InvertedForeground);
	
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}
void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//��������Ʈ ��� - inventory �������� - player�� //find �غ��� �ȵǸ� ��������
	player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	inventoryComponent = (player)->inventoryComponent;
	inventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::UpdateInventory);

	//������ ���� �̹���
	FString itemSlotFramePath = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/bigslot.bigslot'");
	FString itemSlotEmptyPath = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/bigslot.bigslot'");
	itemSlotFrameT = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *itemSlotFramePath));
	itemSlotEmptyT = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *itemSlotEmptyPath));

	//Posion
	ItemBorderHp->SetBrushFromTexture(itemSlotFrameT);
	PosionCoolTimeBar->SetVisibility(ESlateVisibility::Hidden);
	ItemQuantity1->SetVisibility(ESlateVisibility::Hidden);

	//Sword
	ItemBorderSword->SetBrushFromTexture(itemSlotFrameT);
	ItemQuantitySword->SetVisibility(ESlateVisibility::Hidden);

	//Bow
	ItemBorderBow->SetBrushFromTexture(itemSlotFrameT);
	ItemQuantityBow->SetVisibility(ESlateVisibility::Hidden);
}