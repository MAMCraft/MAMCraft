// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_OnAttackEnd.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAnimNotify_OnAttackEnd : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};

