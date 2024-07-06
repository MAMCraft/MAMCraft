// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_AttackRangeCheck.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UBTService_AttackRangeCheck : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "TRACE")
	float traceRange = 100.0f;
	UPROPERTY(EditAnywhere,Category = "TRACE")
	float traceRadius = 26.0f;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
