// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"


void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	auto ownerPawn = TryGetPawnOwner();
	auto player = Cast<APlayerCharacter>(ownerPawn);

	// 캐스팅에 성공했다면
	if (player)
	{
		FVector velocity = player->GetVelocity();
		FVector forwardVector = player->GetActorForwardVector();
		MovementSpeed = FVector:: DotProduct(forwardVector, velocity);
	}
}
