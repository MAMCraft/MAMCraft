// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerZombie.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "EnemyZombiePawn.h"

//const FName AAIControllerZombie::HomePosKey(TEXT("HomePos"));
//const FName AAIControllerZombie::PatrolPosKey(TEXT("PatrolPos"));


AAIControllerZombie::AAIControllerZombie()
{

	static ConstructorHelpers::FObjectFinder<UBlackboardData> bBObject(TEXT("/Script/AIModule.BlackboardData'/Game/LSJ/AI/BB_EnemyZombie.BB_EnemyZombie'"));
	if (bBObject.Succeeded())
	{
		bBAsset = bBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> bTObject(TEXT("/Script/AIModule.BehaviorTree'/Game/LSJ/AI/BT_EnemyZombie.BT_EnemyZombie'"));
	if (bTObject.Succeeded())
	{
		bTAsset = bTObject.Object;
	}
}

void AAIControllerZombie::RunAI()
{
	if (UBehaviorTree* const tree = bTAsset)
	{
		UBlackboardComponent* b;
		UseBlackboard(tree->BlackboardAsset, b);
		Blackboard = b;
		RunBehaviorTree(tree);
	}
}


void AAIControllerZombie::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();

	/*Super::OnPossess(InPawn);
	if (InPawn == nullptr) return;

	AEnemyZombiePawn* enemy = Cast<AEnemyZombiePawn>(InPawn);
	if (AEnemyZombiePawn)
	{
		if (AEnemyZombiePawn->GetBehaviorTree())
		{
			BlackboardComponent->InitializeBlackboard(*(enemy->GetBehaviorTree()->BlackboardAsset));
		}
	}*/
}

void AAIControllerZombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//if (BrainComponent == nullptr)
	//{
	//	BrainComponent = FindComponentByClass<UBrainComponent>();
	//}
	//if (Blackboard == nullptr)
	//{
	//	Blackboard = FindComponentByClass<UBlackboardComponent>();
	//}
}
