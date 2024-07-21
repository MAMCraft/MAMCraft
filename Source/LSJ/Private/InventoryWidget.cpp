// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"
#include "MAMCGameModeBase.h"
void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (gi->GetHPCooldownRemainTime() > 0.1f)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(gi->instanceHPCooldownTimerHandle))
		{
			PosionCoolTimeBar->SetVisibility(ESlateVisibility::Visible);
			PosionCoolTimeBar->SetPercent(GetWorld()->GetTimerManager().GetTimerRemaining(gi->instanceHPCooldownTimerHandle) / 60.0f);
		}
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(player->HPCooldownTimerHandle))
		{
			PosionCoolTimeBar->SetVisibility(ESlateVisibility::Visible);
			UpdatePosionCoolTimeUpdate();
		}

	}
		
}

void UInventoryWidget::UpdatePosionCoolTimeUpdate()
{
	PosionCoolTimeBar->SetPercent(GetWorld()->GetTimerManager().GetTimerRemaining(player->HPCooldownTimerHandle)/60.0f);
}

void UInventoryWidget::AddToViewport()
{
	Super::AddToViewport();
	gi = Cast<UMAMCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gi && gi->GetHPCooldownRemainTime()>1.0f)
		bNextLevel = true;
	//FString CurrentMapName = GetWorld()->GetMapName();
	////if (CurrentMapName.Equals(TEXT("UEDPIE_0_HMap")))
	////{
	////	/*	UMAMCGameInstance* GI = Cast<UMAMCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	////		if (GI)
	////			DefaultItems = GI->LoadInventory();*/
	////	AMAMCGameModeBase* gamemode = Cast<AMAMCGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	////	gamemode->Load();
	////}
	Items = inventoryComponent->Items;
	if (!Items.IsEmpty())
	{
		//인벤토리에 넣어주기
		for (auto* item : Items)
		{
			if (item->category == (int)EItemCategroy::arrow)
			{
				ItemQuantityArrow->SetText(FText::AsNumber(item->count));
				ArrowEmptyImage->SetVisibility(ESlateVisibility::Hidden);
				switch (item->count)
				{
				case -1:ItemQuantityArrow->SetText(FText::AsNumber(0));
				case 0:ItemIconArrow_1->SetVisibility(ESlateVisibility::Hidden);
				case 1:ItemIconArrow_2->SetVisibility(ESlateVisibility::Hidden);
				case 2:ItemIconArrow_3->SetVisibility(ESlateVisibility::Hidden);
				case 3:ItemIconArrow_4->SetVisibility(ESlateVisibility::Hidden);
				case 4:ItemIconArrow_5->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}
	if (GetVisibility() != ESlateVisibility::Hidden)
	{
		UpdateInventory();
	}
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
		//ItemQuantity1->SetText(FText::AsNumber(0));
		//FSlateColor InvertedForeground = FLinearColor(0, 0, 0);
		//ItemIcon1->SetBrushTintColor(InvertedForeground);
		return;
	}
	//인벤토리에 넣어주기
	for (auto* item : Items)
	{
		switch (item->category)
		{
		case (int)EItemCategroy::posion:
			ItemQuantity1->SetText(FText::AsNumber(item->count)); //??
			ItemIconHp->SetBrushFromTexture(posionTexture);
			break;
		case (int)EItemCategroy::sword:
			ItemQuantitySword->SetText(FText::AsNumber(item->count));
			ItemIconSword->SetBrushFromTexture(swordTexture);
			break;
		case (int)EItemCategroy::bow:
			ItemQuantityBow->SetText(FText::AsNumber(item->count));
			if(item->itemID==1)
				ItemIconBow->SetBrushFromTexture(bowBasicTexture);
			else
				ItemIconBow->SetBrushFromTexture(bowBubbleTexture);
			break;
		case (int)EItemCategroy::arrow:
			if (item->count == -1)
			{
				ItemQuantityArrow->SetText(FText::AsNumber(0));
			}
			else
			{
				ItemQuantityArrow->SetText(FText::AsNumber(item->count));
				ArrowEmptyImage->SetVisibility(ESlateVisibility::Hidden);
			}
			if(item->count<5)
				ArrowPlayAnimation(item->count);
			if (item->count >= 5)
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
	//UMAMCGameInstance* GI = Cast<UMAMCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (GI)
	//	GI->SaveInventory(Items);
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
	FString Path5 = TEXT("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/Hp/potion_empty.potion_empty'");
	posionTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path5));
	//Sword
	ItemBorderSword->SetBrushFromTexture(itemSlotFrameT);
	ItemQuantitySword->SetVisibility(ESlateVisibility::Hidden);
	FString Path1 = TEXT("/Script/Engine.Texture2D'/Game/GameResource/Player/Sword/T_Sword_Steel_Icon_inventory.T_Sword_Steel_Icon_inventory'");
	swordTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path1));
	//Bow
	ItemBorderBow->SetBrushFromTexture(itemSlotFrameT);
	ItemQuantityBow->SetVisibility(ESlateVisibility::Hidden);
	FString Path2 = TEXT("/Script/Engine.Texture2D'/Game/GameResource/Player/Bow/T_Bow_Icon_inventory.T_Bow_Icon_inventory'");
	bowBasicTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path2));
	FString Path4 = TEXT("/Script/Engine.Texture2D'/Game/GameResource/Player/Bow/T_BubbleBow_Spooky2_Icon_Inventory.T_BubbleBow_Spooky2_Icon_Inventory'");
	bowBubbleTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path4));
	//Arrow
	ArrowBackground->SetBrushFromTexture(arrowSlotFrameT);
	ItemQuantityArrow->SetVisibility(ESlateVisibility::Visible);
	ArrowEmptyImage->SetBrushFromTexture(arrowEmptySlotT);
	ArrowEmptyImage->SetVisibility(ESlateVisibility::Hidden);
	ArrowRedFrame->SetBrushFromTexture(ItemArrowRedFrameT);
	ArrowRedFrame->SetVisibility(ESlateVisibility::Hidden);
	FString Path3 = TEXT("/Script/Engine.Texture2D'/Game/GameResource/Player/UI/arrow.arrow'");
	arrowTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path3));
}