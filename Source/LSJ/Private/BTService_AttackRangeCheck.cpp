// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AttackRangeCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include <Kismet/GameplayStatics.h>


void UBTService_AttackRangeCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	/** Tag used to indicate an owner for this trace */
	//FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), true, this);
	APawn* controllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	FCollisionQueryParams Params(NAME_None,true, controllingPawn);
	FVector currentLocation = controllingPawn->GetActorLocation();
	FVector attackRangeLocation = currentLocation + controllingPawn->GetActorForwardVector() * traceRange;
	FHitResult HitResult;


	//FCollisionQueryParams Params(EName::Name, false, this);
	//����� ä�η� ��ȯ
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		currentLocation,
		attackRangeLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(50.0f),
		Params);


	FVector traceVec = controllingPawn->GetActorForwardVector() * traceRange;
	FVector center = currentLocation + traceVec * 0.5f;
	//������
	float halfHeight = traceRange * 0.5f + traceRadius;
	//ĸ�� ȸ������
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	//Ÿ�� �߽߰� ���,�̹߽߰� ����
	FColor drawColor = bResult ? FColor::Green : FColor::Red;
	//������ �����Ǳ������ �ð�
	float debugLifeTime = 5.0f;
	DrawDebugCapsule(GetWorld(), center, halfHeight, traceRadius,capsuleRot, drawColor, false, debugLifeTime);

	if (bResult)
	{
		UE_LOG(LogTemp, Display, TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetName());
		if (HitResult.GetActor() == UGameplayStatics::GetPlayerPawn(this, 0))
		{
			//�����Ѵٸ� �α� �����
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("InAttackRange"), true);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("InAttackRange"), false);
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("InAttackRange"), false);
	}
	//DrawDebugSphere(controllingPawn->GetWorld(), currentLocation, traceRange, 16, FColor::Red, false, 0.2f);
		//const UWorld* InWorld, FVector const& Center, FVector const& Box, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness
	//DrawDebugBox(GetWorld(), currentLocation, FVector(150.0f, 50.0f, 50.0f), FColor::Red, false, 0.2f);
}
