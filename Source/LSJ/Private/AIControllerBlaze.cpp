// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBlaze.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "EnemyBlazePawn.h"

AAIControllerBlaze::AAIControllerBlaze(FObjectInitializer const& objectInitializer)
{
	/*static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT(""));
	if (obj.Succeeded())
	{
		btree = obj.Object;
	}
	behavior_tree_component = objectInitializer.CreateDefaultSubobject(<UBehaviorTreeComponent>(this, TEXT(""));*/
}

void AAIControllerBlaze::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AEnemyBlazePawn* const blaze = Cast<AEnemyBlazePawn>(InPawn))
	{
		if (UBehaviorTree* const tree = blaze->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}

}

void AAIControllerBlaze::OnUnPossess()
{
	Super::OnUnPossess();
	
}

void AAIControllerBlaze::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AAIControllerBlaze::RunAI()
{
	
}
