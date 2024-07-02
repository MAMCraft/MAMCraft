// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyAnimationAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyAnimationAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LSJ_API IEnemyAnimationAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AttackHitCheck() = 0;
	virtual void Attack()=0;
	virtual void Attack(TArray<FVector>& location) = 0;
	virtual void AttackHitEnd() = 0;
	virtual void OnAttackEnd() = 0;
};
