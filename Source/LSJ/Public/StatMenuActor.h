// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIStartMenu.h"
#include "StatMenuActor.generated.h"

UCLASS()
class LSJ_API AStatMenuActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatMenuActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
		UPROPERTY()
	TSubclassOf<UUserWidget> MainHUDWidgetClass;
	UUIStartMenu* MainHUDWidget;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
