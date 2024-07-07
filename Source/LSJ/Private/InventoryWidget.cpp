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
	//�ݺ��� or �ϳ��ϳ� �� ����
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

	//��������Ʈ ��� - inventory �������� - player�� //find �غ��� �ȵǸ� ��������
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	inventoryComponent = (player)->inventoryComponent;
	inventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::UpdateInventory);

}