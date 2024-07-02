// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceBlaze.h"

UAnimInstanceBlaze::UAnimInstanceBlaze()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/LSJ/Resource/Blaze/Animaion/blazeAnimationAttack_SK_Blaze_ao_Blaze_BasicAttackTrippleAttack_Montage.blazeAnimationAttack_SK_Blaze_ao_Blaze_BasicAttackTrippleAttack_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		attackMontage = ATTACK_MONTAGE.Object;
	}
	else
		UE_LOG(LogTemp, Display, TEXT("attackMontage nullptr"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HIT_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/LSJ/Resource/Blaze/Animaion/blazeAnimationHitorStun_SK_Blaze_ao_Blaze_Stun_Montage.blazeAnimationHitorStun_SK_Blaze_ao_Blaze_Stun_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		hitMontage = HIT_MONTAGE.Object;
	}
	else
		UE_LOG(LogTemp, Display, TEXT("hitMontage nullptr"));
}
void UAnimInstanceBlaze::NativeInitializeAnimation()
{
	//Super::NativeInitializeAnimation();
	if (pawn == nullptr)
	{
		// 현재 스켈레톤의 주인(폰)을 가져온다.
		pawn = TryGetPawnOwner();
	}
}
void UAnimInstanceBlaze::UpdateProperties()
{
}

void UAnimInstanceBlaze::PlayAttackMontage()
{
	Montage_Play(attackMontage, 1.0f);
}

void UAnimInstanceBlaze::PlayHitMontage()
{
	Montage_Play(hitMontage, 1.0f);
}

void UAnimInstanceBlaze::AnimNotify_Fire()
{
	BulletFire.Broadcast();
}
