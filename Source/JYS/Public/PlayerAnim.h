// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FEndAttack);
/**
 * 
 */
UCLASS()
class JYS_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

	UPlayerAnim();
public:
			UPROPERTY()
	class UAnimMontage* attackComboMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MovementSpeed = 0.0f;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FEndAttack EndAttack;
	UFUNCTION()
	virtual void AnimNotify_NextAttackCheck();
		UFUNCTION()
	virtual void AnimNotify_EndAttack();
	virtual FName GetAttackMontageSectionName(int32 Section);
	virtual void PlayAttackMontage();
	virtual void JumpToAttackMontageSection(int32 NewSection);
	virtual void JumpToAttackMontageSection1();
	virtual void JumpToAttackMontageSection2();
};
