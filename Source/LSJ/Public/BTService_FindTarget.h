// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_FindTarget.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UBTService_FindTarget : public UBTService_BlackboardBase
{ //UBTService
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "TRACE")
	float traceRadius = 1000.0f;
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
