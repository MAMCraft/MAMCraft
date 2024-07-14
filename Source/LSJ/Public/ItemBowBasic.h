// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemBowBasic.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API AItemBowBasic : public AItem
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay();
public:
	AItemBowBasic();

};
