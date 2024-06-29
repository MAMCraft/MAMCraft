// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BB_ChasePlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_BB_ChasePlayer::UBTTask_BB_ChasePlayer(FObjectInitializer const& object_initializer)
{
	NodeName = "Chase Player";
}

EBTNodeResult::Type UBTTask_BB_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const cont = OwnerComp.GetAIOwner();
	FVector const playerLocation = cont->GetBlackboardComponent()->GetValueAsVector(FName("PlayerPos"));
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, playerLocation);
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
