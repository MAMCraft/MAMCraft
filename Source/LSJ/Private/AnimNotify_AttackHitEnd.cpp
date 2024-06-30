// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackHitEnd.h"
#include "EnemyAnimationAttackInterface.h"

void UAnimNotify_AttackHitEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IEnemyAnimationAttackInterface* attackPawn = Cast<IEnemyAnimationAttackInterface>(MeshComp->GetOwner());
		if (attackPawn)
		{
			attackPawn->AttackHitEnd();
		}
	}
}