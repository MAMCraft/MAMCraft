// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class LSJ_API ANPC : public ACharacter
{
	GENERATED_BODY()
	bool isAttacking = false;
public:
	// Sets default values for this character's properties
	ANPC();
	UBehaviorTree* GetBehaviorTree() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI",meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ANIMATION", Meta = (AllowPrivateAccess = true))
	UAnimInstance* AnimInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PAWN", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
public:
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruped);
};
