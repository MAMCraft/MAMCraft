// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>

#include "ItemUserWidget.generated.h"


/**
 * 
 */
UCLASS()
class LSJ_API UItemUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UImage* ItemImage;
	void BindItemImage(UTexture2D* texture);
	FPointerEventHandler imageHandler;
};
