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
		dieMontage = HIT_MONTAGE.Object;
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
	// 폰이 없어졌다면?
	if (pawn == nullptr)
	{
		// 다시 폰을 가져온다.
		pawn = TryGetPawnOwner();
	}

	// 폰이 존재한다면,
		// 애님이 실행되기 전 폰이 삭제된다면, 
		// 없는 객체를 참조할 수 있기에 아래와 같이 if문으로 묶어주었다.
	if (pawn)
	{
		// Movement Speed Check
		FVector Velocity = pawn->GetVelocity();

		// Z축은 필요가 없으니, 0으로 초기화해준다.
		FVector NewVelocity = FVector(Velocity.X, Velocity.Y, 0.f);
		if (pawn->GetActorLocation() != previousLocation)
			movementSpeed = 2;
		else
			movementSpeed = 0;
		//movementSpeed = NewVelocity.Size();
		previousLocation = pawn->GetActorLocation();
	}
}

void UAnimInstanceBlaze::PlayAttackMontage()
{
	Montage_Play(attackMontage, 1.0f);
}

void UAnimInstanceBlaze::PlayDieMontage()
{
	Montage_Play(dieMontage, 1.0f);
}

void UAnimInstanceBlaze::AnimNotify_Fire()
{
	BulletFire.Broadcast();
}
