// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "MAMCGameInstance.h"
#include "MAMCGameModeBase.h"
#include <Kismet/GameplayStatics.h>
//시작 아이템 가져오기
void UInventoryComponent::StartItem()
{
	if (Items.IsEmpty())
	{
		AItem* itemActor = GetWorld()->SpawnActor<AIncreaseHPItem>(itemHp);
		Items.Add(itemActor);
		itemActor->Destroy();
		itemActor =GetWorld()->SpawnActor<AItemSword>(itemSword);
		Items.Add(itemActor);
		itemActor->Destroy();
		itemActor =GetWorld()->SpawnActor<AItemBowBasic>(itemBowBasic);
		Items.Add(itemActor);
		itemActor->Destroy();
		itemActor =GetWorld()->SpawnActor<AArrowItem>(itemArrow);
		Items.Add(itemActor);
		itemActor->Destroy();
	
		Items[3]->count = 10;
		FString CurrentMapName = GetWorld()->GetMapName();
		if (CurrentMapName.Equals(TEXT("UEDPIE_0_HMap")))
		{
			UMAMCGameInstance* gi = Cast<UMAMCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			gi->Load(Items);
		}
	}
}

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<AIncreaseHPItem> itemHpClassFinder(TEXT("/Script/Engine.Blueprint'/Game/JYS/Blueprints/BP_IncreaseHPItem.BP_IncreaseHPItem_C'"));
	if (itemHpClassFinder.Succeeded())
	{
		itemHp = itemHpClassFinder.Class;
	}
	static ConstructorHelpers::FClassFinder<AItemBowBasic> itemBowBasicClassFinder(TEXT("/Script/Engine.Blueprint'/Game/LSJ/Blueprints/Inventory/BP_BowBasic.BP_BowBasic_C'"));
	if (itemBowBasicClassFinder.Succeeded())
	{
		itemBowBasic = itemBowBasicClassFinder.Class;
	}
	static ConstructorHelpers::FClassFinder<AItemSword> itemSwordClassFinder(TEXT("/Script/Engine.Blueprint'/Game/LSJ/Blueprints/Inventory/BP_Sword.BP_Sword_C'"));
	if (itemSwordClassFinder.Succeeded())
	{
		itemSword = itemSwordClassFinder.Class;
	}
	static ConstructorHelpers::FClassFinder<AArrowItem> itemArrowClassFinder(TEXT("/Script/Engine.Blueprint'/Game/JYS/Blueprints/BP_ArrowItem.BP_ArrowItem_C'"));
	if (itemArrowClassFinder.Succeeded())
	{
		itemArrow = itemArrowClassFinder.Class;
	}
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	StartItem();
	AMAMCGameModeBase* gamemode = Cast<AMAMCGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	gamemode->VisibleInventoryWidget();
	// ...
	////////////////////////////////////////////////// initcomponent
	//OnInventoryUpdated.Broadcast();
	//for (auto* Item : DefaultItems)
	//{
	//	AddItem(Item);
	//}
	
}

bool UInventoryComponent::AddItem(AItem* Item)
{
	for (AItem*& findItem : Items)
	{
		if (nullptr == findItem)
			continue;
		if (findItem->category == Item->category) //ItemDisplayName 으로 검색
		{
			//아이템설명
			//findItem->UseActionText = Item->UseActionText;
			//텍스트 지정
			//findItem->Thumnail = Item->Thumnail;
			//아이템명 으로 탐색 for문
			//findItem->Tags = Item->Tags;
			findItem->category = Item->category;
			findItem->count++;
			findItem->itemID = Item->itemID;
			OnInventoryUpdated.Broadcast();
			return true;
		}
		//if (findItem->ItemDisplayName.EqualTo(Item->ItemDisplayName)) //ItemDisplayName 으로 검색
		//{
		//	findItem->count++;
		//	OnInventoryUpdated.Broadcast();
		//	return true;
		//}
	}
	Item->OwningInventory = this;
	Item->World = GetWorld();
	Item->count++;
	Items.Add(Item);
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

EBowCategory UInventoryComponent::GetCurrentBow()
{

	if(Items[(int)EItemCategroy::bow]->itemID == 1)//EqualTo(FText::FromString("ItemBowBasic")))
		return EBowCategory::basic;
	else
		return EBowCategory::bubble;
}

void UInventoryComponent::ArrowUsed(int amount)
{
	Items[(int)EItemCategroy::arrow]->count -= amount;
	OnInventoryUpdated.Broadcast();
	if (Items[(int)EItemCategroy::arrow]->count == 0)
		Items[(int)EItemCategroy::arrow]->count = -1;
}

void UInventoryComponent::AddArrow(int amount)
{
	if (Items[(int)EItemCategroy::arrow]->count == -1)
		Items[(int)EItemCategroy::arrow]->count = 0;
	Items[(int)EItemCategroy::arrow]->count += amount;
	OnInventoryUpdated.Broadcast();
}

int UInventoryComponent::GetArrow()
{
	return Items[(int)EItemCategroy::arrow]->count;
}



