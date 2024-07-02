// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class JYS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* rollingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* attackComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* bowMontage;

	UFUNCTION(BlueprintCallable)
	void OnMyTakeDamage(int damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int playerMaxHP = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int playerHP = playerMaxHP;

	// Event handler for montage notify begin
	UFUNCTION()
	void HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingpayload);

	void comboAttack();
	bool isAttacking;
	int ComboAttackIndex = 0;

	//void attack();

	void skill();

	void Rolling();

private:

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* cameraComponent;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* springArmComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> HPWidgetFactory;

	UPROPERTY()
	class UHPWidget* HPWidget;

};
