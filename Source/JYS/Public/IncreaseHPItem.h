// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "IncreaseHPItem.generated.h"

UCLASS()
class JYS_API AIncreaseHPItem : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIncreaseHPItem();

protected:
	virtual void BeginPlay();
};

