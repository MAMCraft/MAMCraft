// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyZombieController.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyZombie.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AEnemyZombieController::HomePosKey(TEXT("HomePos"));
const FName AEnemyZombieController::PatrolPosKey(TEXT("PatrolPos"));

AEnemyZombieController::AEnemyZombieController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Script/AIModule.BehaviorTree'/Game/LSJ/BehaviorTree/BT_Enemy.BT_Enemy'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Script/AIModule.BlackboardData'/Game/LSJ/BehaviorTree/BB_Enemy.BB_Enemy'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}
void AEnemyZombieController::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AEnemyZombieController::GoToNextPatrolPoint, 2.0f);
	RepeatInterval = 3.0f;
}
void AEnemyZombieController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AEnemyZombieController::OnRepeatTimer, RepeatInterval, true);
	//Blackboard = FindComponentByClass<UBlackboardComponent>();
	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		// 액터의 위치정보를 HomePosKey에 저장
		BlackboardComp->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Log, TEXT("AIController coudn't run behavior tree!"));
		}
	}
}
void AEnemyZombieController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}
void AEnemyZombieController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr != NavSystem) return;

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
	}
}



/// <summary>
/// 위
/// </summary>
void AEnemyZombieController::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	//SetFocus(playerPawn);
	//MoveToActor(playerPawn, 1);
	
	//이동
	
	//길찾기

	//카메러 시야
}

void AEnemyZombieController::OnMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result)
{
	Super::OnMoveCompleted(requestID, result);
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AEnemyZombieController::GoToNextPatrolPoint, 2.0f);
}

void AEnemyZombieController::GoToNextPatrolPoint()
{
	AEnemyZombie* enemy = Cast<AEnemyZombie>(GetPawn());
	if (enemy)
	{
		MoveToLocation(enemy->GetNextPatrolPoint());
	}
}

void AEnemyZombieController::SetNewDestination(const FVector destination)
{
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (playerPawn)
	{
		float const distance = FVector::Dist(destination, playerPawn->GetActorLocation());
		if (distance>120.f)
		{
			 
		}
	}
}
