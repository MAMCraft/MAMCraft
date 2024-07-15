// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FailScreen.generated.h"

/**
 * 
 */
UCLASS()
class MAMCRAFT_API UFailScreen : public UUserWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void SetDeathText(const FText& DeathText);

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* DeathMessageTextBlock;
};
