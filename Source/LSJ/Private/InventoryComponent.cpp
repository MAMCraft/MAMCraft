// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

void UInventoryComponent::StartItem()
{
	DefaultItems.Add(GetWorld()->SpawnActor<AIncreaseHPItem>(itemHp));
	DefaultItems.Add(GetWorld()->SpawnActor<AItemSword>(itemSword));
	DefaultItems.Add(GetWorld()->SpawnActor<AItemBowBasic>(itemBowBasic));
}

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FClassFinder<AIncreaseHPItem> itemHpClassFinder(TEXT("/Script/Engine.Blueprint'/Game/JYS/Blueprints/BP_IncreaseHPItem.BP_IncreaseHPItem_C'"));
	if (itemHpClassFinder.Succeeded())
	{
		itemHp = itemHpClassFinder.Class;
	}
	ConstructorHelpers::FClassFinder<AItemBowBasic> itemBowBasicClassFinder(TEXT("/Script/Engine.Blueprint'/Game/LSJ/Blueprints/Inventory/BP_BowBasic.BP_BowBasic_C'"));
	if (itemBowBasicClassFinder.Succeeded())
	{
		itemBowBasic = itemBowBasicClassFinder.Class;
	}
	ConstructorHelpers::FClassFinder<AItemSword> itemSwordClassFinder(TEXT("/Script/Engine.Blueprint'/Game/LSJ/Blueprints/Inventory/BP_Sword.BP_Sword_C'"));
	if (itemSwordClassFinder.Succeeded())
	{
		itemSword = itemSwordClassFinder.Class;
	}
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	StartItem();
	for (auto* Item : DefaultItems)
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
	//포션 아이템은 계속 유지하기 위해 일단은 return
	return true;
	//if (Item)
	//{
	//	Item->count--;
	//	if (Item->count > 0)
	//	{
	//		OnInventoryUpdated.Broadcast();
	//		return true;
	//	}
	//	Item->count = 0;
	//	Item->OwningInventory = nullptr;
	//	Item->World = nullptr;
	//	Items.RemoveSingle(Item);
	//	OnInventoryUpdated.Broadcast();
	//	return true;
	//}
	//return false;
}



