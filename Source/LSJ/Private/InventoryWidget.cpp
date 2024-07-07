// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerCharacter.h"

void UInventoryWidget::AddToViewport()
{
	Super::AddToViewport();

}

void UInventoryWidget::UpdateInventory()
{
	Items = inventoryComponent->Items;
	if (Items.IsEmpty())
	{
		ItemQuantity1->SetText(FText::AsNumber(0));
		FSlateColor InvertedForeground = FLinearColor(0, 0, 0);
		ItemIcon1->SetBrushTintColor(InvertedForeground);
		return;
	}
	//반복문 or 하나하나 다 쓰기
	if (Items[0]->count <= 0)
	{
		return;
	}
	FSlateColor InvertedForeground = FLinearColor(1, 1, 1);
	ItemIcon1->SetBrushTintColor(InvertedForeground);
	ItemQuantity1->SetText(FText::AsNumber(Items[0]->count));
	ItemIcon1->SetBrushFromTexture(Items[0]->Thumnail);
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();


}
void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//델리게이트 등록 - inventory 가져오기 - player의 //find 해보고 안되면 가져오자
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	inventoryComponent = (player)->inventoryComponent;
	inventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::UpdateInventory);

}