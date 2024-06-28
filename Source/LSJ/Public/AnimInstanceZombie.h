// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceZombie.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAnimInstanceZombie : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="MOVEMENT",meta=(AllowPrivateAccess="true"))
	float movementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MOVEMENT", meta=(AllowPrivateAccess="true"))
	class APawn* pawn;
	UFUNCTION(BlueprintCallable, Category = "UPDATE")
	void UpdateProperties();
	UFUNCTION()
	void NativeInitializeAnimation() override;

};
