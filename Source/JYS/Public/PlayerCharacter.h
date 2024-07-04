// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int playerMaxHP = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int playerHP = playerMaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackDamage = 1;

	UFUNCTION(BlueprintCallable)
	void OnMyTakeDamage(int damage);

	// Event handler for montage notify begin
	UFUNCTION()
	void HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingpayload);

	UFUNCTION()
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION()
	void comboAttack();

	bool isAttacking;
	int ComboAttackIndex = 0;

	// 데미지 처리
	UFUNCTION()
	void Hit(AActor* OtherActor);

	void ApplyFireDamage();
	void StopFireDamage();
	void FlashRed();
	void ResetMaterial();

	FTimerHandle FireDamageTimerHandle;
	FTimerHandle FlashRedTimerHandle;

	bool bIsTakingFireDamage;

	void skill();

	void Rolling();

	//weapon
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* rightWeaponCollision;

	UFUNCTION()
	void OnrightWeaponCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	///** Overlap End */
	//UFUNCTION()
	//void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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
