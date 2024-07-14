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
		// ���� ���̷����� ����(��)�� �����´�.
		pawn = TryGetPawnOwner();
	}
}
void UAnimInstanceBlaze::UpdateProperties()
{
	// ���� �������ٸ�?
	if (pawn == nullptr)
	{
		// �ٽ� ���� �����´�.
		pawn = TryGetPawnOwner();
	}

	// ���� �����Ѵٸ�,
		// �ִ��� ����Ǳ� �� ���� �����ȴٸ�, 
		// ���� ��ü�� ������ �� �ֱ⿡ �Ʒ��� ���� if������ �����־���.
	if (pawn)
	{
		// Movement Speed Check
		FVector Velocity = pawn->GetVelocity();

		// Z���� �ʿ䰡 ������, 0���� �ʱ�ȭ���ش�.
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
