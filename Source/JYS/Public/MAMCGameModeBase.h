// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MAMCGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class JYS_API AMAMCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
    virtual void BeginPlay() override;
	//UPROPERTY()
	//TSubclassOf<UUserWidget> MainHUDWidgetClass;
	//UUIStartMenu* MainHUDWidget;
public:
    AMAMCGameModeBase();
	UFUNCTION()
	void EndViewport();
};
