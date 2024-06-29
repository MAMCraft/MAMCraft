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

protected:
	//virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void NativeInitializeAnimation();
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="MOVEMENT", meta = (allowprivateaccess = true))
	float movementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MOVEMENT", meta = (allowprivateaccess = true))
	class APawn* pawn;
	UFUNCTION(BlueprintCallable, Category = "UPDATE")
	void UpdateProperties();


};
