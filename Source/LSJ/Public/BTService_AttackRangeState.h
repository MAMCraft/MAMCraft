// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_AttackRangeState.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UBTService_AttackRangeState : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	bool IsTooClose = false;
	bool IsAttackRange = false;
public:
	UPROPERTY(EditAnywhere, Category = "TRACE")
	float attackMaxRadius = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "TRACE")
	float attackMinRadius = 500.f;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
