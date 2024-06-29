// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindTarget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>

void UBTService_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory,DeltaSeconds);
	APawn* controllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	const FVector startLocation = controllingPawn->GetActorLocation();
	const FVector endLocation = startLocation;

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(controllingPawn);
	TArray<FHitResult> hitArray;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
	objectType.Emplace(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3));
	AActor* target = nullptr;
	const bool hit = UKismetSystemLibrary::SphereTraceMultiForObjects(controllingPawn, startLocation, endLocation, traceRadius, objectType, false, actorsToIgnore, EDrawDebugTrace::ForOneFrame, hitArray, true);
	if (hit)
	{
		for (auto i = 0; i < hitArray.Num(); i++)
		{
			if (hitArray[i].GetActor() == UGameplayStatics::GetPlayerPawn(this, 0))
			{
				target = hitArray[i].GetActor();
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Player"), target);
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("PlayerPos"), target->GetActorLocation());
			}
		}
	}
	if(target==nullptr)
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Player"), nullptr);
	DrawDebugSphere(controllingPawn->GetWorld(), startLocation, traceRadius, 16, FColor::Red, false, 0.2f);
}
