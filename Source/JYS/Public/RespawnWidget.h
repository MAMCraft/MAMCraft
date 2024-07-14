// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Slider.h>
#include "RespawnWidget.generated.h"


/**
 * 
 */
UCLASS()
class JYS_API URespawnWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable, Category="Respawn")
    void SetRespawnCount(int32 CurrentRespawns, int32 MaxRespawns);

protected:
    UPROPERTY(meta = (BindWidget))
    USlider* RespawnSlider;
};