// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceZombie.h"

void UAnimInstanceZombie::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (pawn == nullptr)
	{
		// 현재 스켈레톤의 주인(폰)을 가져온다.
		pawn = TryGetPawnOwner();
	}
}
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

