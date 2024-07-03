// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceZombie.h"

void UAnimInstanceZombie::NativeInitializeAnimation()
{
	//Super::NativeInitializeAnimation();
	if (pawn == nullptr)
	{
		// ���� ���̷����� ����(��)�� �����´�.
		pawn = TryGetPawnOwner();
	}
}
UAnimInstanceZombie::UAnimInstanceZombie()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/LSJ/Resource/Zombie/Animation/zombieAnimationAttack_Montage.zombieAnimationAttack_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		attackMontage = ATTACK_MONTAGE.Object;
	}else
		UE_LOG(LogTemp, Display, TEXT("attackMontage nullptr"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HIT_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/LSJ/Resource/Zombie/Animation/zombieAnimationHitEdit_Montage.zombieAnimationHitEdit_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		hitMontage = HIT_MONTAGE.Object;
	}
	else
		UE_LOG(LogTemp, Display, TEXT("hitMontage nullptr"));
}

void UAnimInstanceZombie::UpdateProperties()
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
		movementSpeed = NewVelocity.Size();
	}
}

void UAnimInstanceZombie::PlayAttackMontage()
{
	Montage_Play(attackMontage, 1.0f);
}

void UAnimInstanceZombie::PlayHitMontage()
{
	Montage_Play(hitMontage, 1.0f);
}