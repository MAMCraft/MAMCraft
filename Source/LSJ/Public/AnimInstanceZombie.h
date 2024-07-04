// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceZombie.generated.h"


DECLARE_MULTICAST_DELEGATE(FHitEnd);
/**
 * 
 */
UCLASS()
class LSJ_API UAnimInstanceZombie : public UAnimInstance
{
	GENERATED_BODY()

protected:
	//virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void NativeInitializeAnimation();
private:
	UAnimInstanceZombie();
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="MOVEMENT", meta = (allowprivateaccess = true))
	float movementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MOVEMENT", meta = (allowprivateaccess = true))
	class APawn* pawn;
	UFUNCTION(BlueprintCallable, Category = "UPDATE")
	void UpdateProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MOVEMENT", meta = (allowprivateaccess = true))
	UAnimMontage* attackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MOVEMENT", meta = (allowprivateaccess = true))
	UAnimMontage* hitMontage;
public:
	UFUNCTION()
	void PlayAttackMontage();
	UFUNCTION()
	void PlayHitMontage();

	UFUNCTION()
	void AnimNotify_HitEnd();
	FHitEnd hitEnd;
};
