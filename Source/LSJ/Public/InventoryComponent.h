// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LSJ_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

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
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="ITEMS")
	TArray<AItem*> Items;
};

