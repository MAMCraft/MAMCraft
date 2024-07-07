// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	for (auto& Item : DefaultItems)
	{
		AddItem(Item);
	}
}

bool UInventoryComponent::AddItem(AItem* Item)
{

	for (AItem* findItem : Items)
	{
		if (findItem->ItemDisplayName.EqualTo(Item->ItemDisplayName))
		{
			findItem->count++;
			OnInventoryUpdated.Broadcast();
			return true;
		}
	}
	Item->OwningInventory = this;
	Item->World = GetWorld();
	Item->count++;
	Items.Add(Item);
	UE_LOG(LogTemp, Error, TEXT("%s"), *Items[0]->UseActionText.ToString());
	OnInventoryUpdated.Broadcast();
	return true;
}

bool UInventoryComponent::RemoveItem(AItem* Item)
{
	if (Item)
	{
		Item->count--;
		if (Item->count > 0)
		{
			OnInventoryUpdated.Broadcast();
			return true;
		}
		Item->count=0;
		Item->OwningInventory = nullptr;
		Item->World = nullptr;
		Items.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}



