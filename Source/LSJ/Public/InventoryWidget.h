// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/Border.h>
#include <Components/ProgressBar.h>
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UTexture2D* itemSlotFrameT;
	UTexture2D* itemSlotEmptyT;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual void NativeOnInitialized();

public:
	//PosionCoolTimeUpdate
	void UpdatePosionCoolTimeUpdate();
	virtual void AddToViewport();
	//Posion
	UPROPERTY(VisibleAnywhere,Category="Drag Item Visual",meta=(BindWidget))
	UProgressBar* PosionCoolTimeBar;
	UPROPERTY(VisibleAnywhere,Category="Drag Item Visual",meta=(BindWidget))
	UBorder* ItemBorderHp;
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UImage* ItemIconHp;			
		UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UTextBlock* ItemQuantity1;
	//Sword
	UPROPERTY(VisibleAnywhere,Category="Drag Item Visual",meta=(BindWidget))
	UBorder* ItemBorderSword;
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UImage* ItemIconSword;
		UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UTextBlock* ItemQuantitySword;
	//Bow
	UPROPERTY(VisibleAnywhere,Category="Drag Item Visual",meta=(BindWidget))
	UBorder* ItemBorderBow;
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UImage* ItemIconBow;
		UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UTextBlock* ItemQuantityBow;
	bool bIsPickingUp = false;

	UFUNCTION(BlueprintCallable)
	void UpdateInventory();
	virtual void NativeConstruct();
	UPROPERTY()
	UInventoryComponent* inventoryComponent;
	TArray<AItem*> Items;
	class APlayerCharacter* player;
};

