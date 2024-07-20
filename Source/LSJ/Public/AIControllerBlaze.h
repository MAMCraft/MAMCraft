// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBlaze.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API AAIControllerBlaze : public AAIController
{
	GENERATED_BODY()
public:
	explicit AAIControllerBlaze(FObjectInitializer const& objectInitializer);
	//class UBlackboardComponent* get_blackboard() const;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void PostInitializeComponents() override;
	void RunAI();
private:
	class UBehaviorTreeComponent* behavior_tree_component;
	class UBehaviorTree* btree;
	class UBlackboardComponent* bbcomp;
};
