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
	//결과를 채널로 반환
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
	//반지름
	float halfHeight = traceRange * 0.5f + traceRadius;
	//캡슐 회전방향
	FQuat capsuleRot = FRotationMatrix::MakeFromZ(traceVec).ToQuat();
	//타겟 발견시 녹색,미발견시 빨강
	FColor drawColor = bResult ? FColor::Green : FColor::Red;
	//생성후 삭제되기까지의 시간
	float debugLifeTime = 5.0f;
	DrawDebugCapsule(GetWorld(), center, halfHeight, traceRadius,capsuleRot, drawColor, false, debugLifeTime);

	if (bResult)
	{
		if (HitResult.GetActor() == UGameplayStatics::GetPlayerPawn(this, 0))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("InAttackRange"), true);
	
			/*		
			FVector direction = HitResult.GetActor()->GetActorLocation() - controllingPawn->GetActorLocation();
			const FRotator resultRotation = FMath::RInterpTo(controllingPawn->GetActorRotation(), 
				QuatRotation, DeltaSeconds, 1.0f);
			controllingPawn->SetActorRotation(resultRotation);*/

			
			//일단 기존에 가지고 있던 Direction과 Angle을 이전 값으로 해준다. 
			FVector PrevGoalDirection= controllingPawn->GetActorForwardVector();
			float PrevTurnAngle= controllingPawn->GetTransform().GetRotation().Euler().Z;
			//먼저 현재 지점에서부터 목표지점을 향한 Direction을 구한다.    
			FVector Dest=FVector(HitResult.GetActor()->GetActorLocation().X, HitResult.GetActor()->GetActorLocation().Y, 0.0f);
			FVector Start=FVector(HitResult.GetActor()->GetTransform().GetLocation().X,HitResult.GetActor()->GetActorLocation().Y, 0.0f);
			FVector newDir=Dest-Start;
			FVector CurrentGoalDirection=newDir.GetSafeNormal();
			//노말라이징한 두개의 백터를 dot한다. //여기서 축을 Z축으로 하기 위해 두백터의 Z값을 0.0f로 넣어 주었다.    
			float dot=FVector::DotProduct(PrevGoalDirection, CurrentGoalDirection);
			float AcosAngle=FMath::Acos(dot);
			// dot한 값을 아크코사인 계산해 주면 0 ~ 180도 사이의 값 (0 ~ 1)의 양수 값만 나온다.    
			float angle=FMath::RadiansToDegrees(AcosAngle);
			//그값은 degrees 값인데 이것에 1라디안을 곱해주면 60분법의 도가 나온다.        
			//여기서 두 백터를 크로스 하여 회전할 축을 얻게 된다.   
			//이 크로스 백터는 Axis회전의 회전축이 되며 , 그 양수 음수로 회전 방향 왼쪽(음수), 오른쪽(양수)를 알수 있다.    
			FVector cross = FVector::CrossProduct(PrevGoalDirection, CurrentGoalDirection);
			FString lr = "center//";
			if (cross.Z >0)
			{
				lr="Right//";
				float NextTurnAngle=angle;
				controllingPawn->GetRootComponent()->SetWorldRotation(FRotator(0.0f, NextTurnAngle, 0.0f).Quaternion());
			}
			else if (cross.Z< 0)
			{
				lr="Left//";
				//NextTurnAngle = 360 - angle; //360에서 뺴게되면 양수로 각을 리턴하게 된다.        
				float floatNextTurnAngle=360-angle;
				controllingPawn->GetRootComponent()->SetWorldRotation(FRotator(0.0f, floatNextTurnAngle, 0.0f).Quaternion());
			}
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
