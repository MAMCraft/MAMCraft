// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerZombie.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "EnemyZombiePawn.h"

//const FName AAIControllerZombie::HomePosKey(TEXT("HomePos"));
//const FName AAIControllerZombie::PatrolPosKey(TEXT("PatrolPos"));


AAIControllerZombie::AAIControllerZombie()
{

	//static ConstructorHelpers::FObjectFinder<UBlackboardData> bBObject(TEXT("/Script/AIModule.BlackboardData'/Game/LSJ/AI/BB_EnemyZombie.BB_EnemyZombie'"));
	//if (bBObject.Succeeded())
	//{
	//	UE_LOG(LogTemp, Display, TEXT("bBObject.Object"));
	//	bBAsset = bBObject.Object;
	//}
	//else
	//	UE_LOG(LogTemp, Display, TEXT("bBObject.Object Fail"));

	//static ConstructorHelpers::FObjectFinder<UBehaviorTree> bTObject(TEXT("/Script/AIModule.BehaviorTree'/Game/LSJ/AI/BT_EnemyZombie.BT_EnemyZombie'"));
	//if (bTObject.Succeeded())
	//{
	//	UE_LOG(LogTemp, Display, TEXT("bTObject.Object"));
	//	bTAsset = bTObject.Object;
	//}
	//else
	//	UE_LOG(LogTemp, Display, TEXT("bTObject.Object Fail"));
}

void AAIControllerZombie::RunAI()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	// BTComp가 NULL이면 UBehaviorTreeComponent 인스턴스를 새롭게 생성한다.
	if (BTComp == NULL)
	{
		UE_VLOG(this, LogBehaviorTree, Log, TEXT("RunBehaviorTree: spawning BehaviorTreeComponent.."));

		BTComp = NewObject<UBehaviorTreeComponent>(this, TEXT("BTComponent"));
		BTComp->RegisterComponent();
	}

	// 앞서 BrainComponent는 NULL일 수도 있고, 아닐 수도 있다.
	// 새로 만든 BTComp는 기존 BrainComponent의 값과 같거나, 새롭게 할당한 값이다.
	// make sure BrainComponent points at the newly created BT component
	BrainComponent = BTComp;

	check(BTComp != NULL);
	BTComp->StartTree(*bTAsset, EBTExecutionMode::Looped);


	/*UBlackboardComponent* blackboardComp = Blackboard;
	if (UseBlackboard(bBAsset, blackboardComp))
	{
		RunBehaviorTree(bTAsset);
	}
	this->Blackboard = blackboardComp;*/
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
