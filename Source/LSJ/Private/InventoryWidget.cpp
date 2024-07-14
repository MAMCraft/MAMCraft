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

void UInventoryWidget::ArrowPlayAnimation(int count)
{
	switch (count)
	{
	case 0:PlayAnimation(arrow1);
		ArrowEmptyImage->SetVisibility(ESlateVisibility::Visible);
		break;
	case 1:PlayAnimation(arrow2);
		break;
	case 2:PlayAnimation(arrow3);
		break;
	case 3:PlayAnimation(arrow4);
		break;
	case 4:PlayAnimation(arrow5);
		break;
	default:
		break;
	}
	
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
	//count가 리턴
	if (Items[0]->count <= 0)
	{
		return;
	}
	//인벤토리에 넣어주기
	for (auto* item : Items)
	{
		switch (item->category)
		{
		case (int)EItemCategroy::posion:
			ItemQuantity1->SetText(FText::AsNumber(Items[0]->count)); //??
			ItemIconHp->SetBrushFromTexture(Items[0]->Thumnail);
			break;
		case (int)EItemCategroy::sword:
			ItemQuantitySword->SetText(FText::AsNumber(Items[1]->count));
			ItemIconSword->SetBrushFromTexture(Items[1]->Thumnail);
			break;
		case (int)EItemCategroy::bow:
			ItemQuantityBow->SetText(FText::AsNumber(Items[2]->count));
			ItemIconBow->SetBrushFromTexture(Items[2]->Thumnail);
			break;
		case (int)EItemCategroy::arrow:
			if (Items[3]->count == -1)
			{
				ItemQuantityArrow->SetText(FText::AsNumber(0));
			}
			else
			{
				ItemQuantityArrow->SetText(FText::AsNumber(Items[3]->count));
				ArrowEmptyImage->SetVisibility(ESlateVisibility::Hidden);
			}
			if(Items[3]->count<5)
				ArrowPlayAnimation(Items[3]->count);
			if (Items[3]->count >= 5)
			{
				ItemIconArrow_1->SetRenderTranslation(FVector2D(0, 0));
				ItemIconArrow_2->SetRenderTranslation(FVector2D(1, 17));
				ItemIconArrow_3->SetRenderTranslation(FVector2D(1, 17));
				ItemIconArrow_4->SetRenderTranslation(FVector2D(1, 40));
				ItemIconArrow_5->SetRenderTranslation(FVector2D(1, 40));
			}
			break;
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

	//델리게이트 등록 - inventory 가져오기 - player의 //find 해보고 안되면 가져오자
	player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	inventoryComponent = (player)->inventoryComponent;
	inventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::UpdateInventory);

	//아이템 슬롯 이미지
	FString itemSlotFramePath = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/bigslot.bigslot'");
	//FString itemSlotEmptyPath = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/bigslot.bigslot'");
	itemSlotFrameT = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *itemSlotFramePath));
	//itemSlotEmptyT = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *itemSlotEmptyPath));
	//화살 슬롯 이미지
	FString arrowSlotFramePath = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/arrow_slot.arrow_slot'");
	FString arrowEmptySlotFramePath = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/arrow_slot_frame.arrow_slot_frame'");
	UTexture2D* arrowSlotFrameT = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *arrowSlotFramePath));
	FString arrowEmptySlotPath = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/arrows_empty.arrows_empty'");
	UTexture2D* arrowEmptySlotT = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *arrowEmptySlotPath));
	UTexture2D* ItemArrowRedFrameT = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *arrowEmptySlotFramePath));
	FString ItemIconArrowPath = FString("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/arrow.arrow'");
	UTexture2D* ItemIconArrowT = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *ItemIconArrowPath));
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

	//Arrow
	ArrowBackground->SetBrushFromTexture(arrowSlotFrameT);
	ItemQuantityArrow->SetVisibility(ESlateVisibility::Visible);
	ArrowEmptyImage->SetBrushFromTexture(arrowEmptySlotT);
	ArrowEmptyImage->SetVisibility(ESlateVisibility::Hidden);
	ArrowRedFrame->SetBrushFromTexture(ItemArrowRedFrameT);
	ArrowRedFrame->SetVisibility(ESlateVisibility::Hidden);
	//ItemIconArrow->SetBrushFromTexture(ItemIconArrowT);
}