// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ArrowItem.generated.h"

UCLASS()
class JYS_API AArrowItem : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrowItem();
protected:
	virtual void BeginPlay();
};

