// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:

	virtual void NativeOnInitialized();

public:
	virtual void AddToViewport();

	UPROPERTY(VisibleAnywhere,Category="Drag Item Visual",meta=(BindWidget))
	class UBorder* ItemBorder1;
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UImage* ItemIcon1;				
		UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UTextBlock* ItemQuantity1;

	bool bIsPickingUp = false;

	UFUNCTION(BlueprintCallable)
	void UpdateInventory();
	virtual void NativeConstruct();
	UPROPERTY()
	UInventoryComponent* inventoryComponent;
	TArray<AItem*> Items;
};

