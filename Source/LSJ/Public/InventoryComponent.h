// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"
#include "IncreaseHPItem.h"
#include "ItemSword.h"
#include "ItemBowBasic.h"
#include "ArrowItem.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
UENUM()
enum class EBowCategory : int
{
	basic,
	bubble,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LSJ_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	void StartItem();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf <AIncreaseHPItem> itemHp;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf <AItemSword> itemSword;
	UPROPERTY(EditDefaultsOnly)	  
	TSubclassOf <AItemBowBasic> itemBowBasic;
	UPROPERTY(EditDefaultsOnly)	  
	TSubclassOf <AArrowItem> itemArrow;
public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	bool AddItem(AItem* Item);
	bool RemoveItem(AItem* Item);

	UPROPERTY(Instanced)
	TArray<AItem*> DefaultItems;
	UPROPERTY()
	int32 Capacity;
	UPROPERTY(BlueprintAssignable)
	FOnInventoryUpdated OnInventoryUpdated;

	TArray<AItem*> Items;

	EBowCategory GetCurrentBow();
	void ArrowUsed(int amount);
	int GetArrow();
	void AddArrow(int amount);
};

