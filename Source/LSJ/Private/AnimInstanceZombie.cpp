// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceZombie.h"

void UAnimInstanceZombie::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (pawn == nullptr)
	{
		// ���� ���̷����� ����(��)�� �����´�.
		pawn = TryGetPawnOwner();
	}
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

