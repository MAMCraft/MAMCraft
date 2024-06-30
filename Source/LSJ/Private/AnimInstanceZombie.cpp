// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceZombie.h"

void UAnimInstanceZombie::NativeInitializeAnimation()
{
	//Super::NativeInitializeAnimation();
	if (pawn == nullptr)
	{
		// 현재 스켈레톤의 주인(폰)을 가져온다.
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
}
//void UAnimInstanceZombie::NativeUpdateAnimation(float DeltaSeconds)
//{
//	Super::NativeUpdateAnimation(DeltaSeconds);
//
//	// 현재 AnimInstance를 소유중인 Pawn을 가져오는 것을 시도함
//	pawn = TryGetPawnOwner();
//
//	// '시도' 이기 때문에 못 가져올 수도 있으므로 유효한 값인지 검사
//	if (IsValid(pawn))
//	{
//		movementSpeed = pawn->GetVelocity().Size();
//	}
//}
void UAnimInstanceZombie::UpdateProperties()
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
		movementSpeed = NewVelocity.Size();
	}
}

void UAnimInstanceZombie::PlayAttackMontage()
{
	Montage_Play(attackMontage, 1.0f);
}
