// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"
#include "PlayerAnim.h"
#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "FailScreen.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class JYS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	int32 GetRespawnCount() const { return RespawnCount; }
	void SetRespawnCount(int32 val) { RespawnCount = val; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class USoundWave* onearrow;
	class USoundWave* threearrow;
	class USoundWave* twosword;
	class USoundWave* threesword;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* rollingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* attackComboMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* bowMontage;
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* runMontage;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int playerMaxHP = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int playerHP = playerMaxHP;

	UFUNCTION()
	void IncreaseHP(int32 Amount);

	UFUNCTION()
	void IncreaseAttackDamage(float Amount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackDamage = 1;

	UFUNCTION(BlueprintCallable)
	void OnMyTakeDamage(int damage);

	// Event handler for montage notify begin
	UFUNCTION()
	void HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingpayload);

	UFUNCTION()
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	bool isAttacking;
	//콤보 공격 적용//////////////////////////
	bool IsAttacking;
	bool CanNextCombo;
	bool IsComboInputOn;
	int32 CurrentCombo;
	int32 MaxCombo;
	UFUNCTION()
	void OnAttackComboMontageEnded();
	void AttackStartComboState();
	void AttackEndComboState();
	UPROPERTY()
	UPlayerAnim* AnimInstance;
	/// /////////////////////////////////////////////////
	int ComboAttackIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Arrow")
    int32 bowState;

    UPROPERTY(EditAnywhere, Category = "Arrow")
    int32 arrowCount;

	UFUNCTION()
	void UpgradeSkill();

	UFUNCTION()
	void FireArrow(const FVector& Direction);

	void skill();

	void Rolling();

	//weapon
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* rightWeaponCollision;

	UFUNCTION()
	void OnrightWeaponCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* cameraComponent;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* springArmComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> HPWidgetFactory;

	UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class UUserWidget> RespawnWidgetClass;


	UPROPERTY()
	class UHPWidget* HPWidget;

	FVector StartLocation; // 캐릭터 시작 위치 저장
	FRotator StartRotation; // 캐릭터 시작 회전 저장

	int32 RespawnCount; // 현재 부활 횟수
	const int32 MaxRespawns = 2; // 최대 부활 횟수

	// 원래 머티리얼을 저장할 변수
	UMaterialInterface* OriginalMaterial;
	UMaterialInterface* RedMaterial;


	// 타이머 핸들러
	FTimerHandle DamageBlinkTimerHandle;

	// 깜빡임 함수
	void BlinkRed();

	// 깜빡임을 종료하는 함수
	void EndBlink();

	// 화염 데미지를 받고 있는지 여부를 나타내는 변수
	bool bIsTakingFireDamage;

	// 타이머 핸들러
	FTimerHandle FireDamageTimerHandle;
	FTimerHandle StopFireDamageTimerHandle;

	// 화염 데미지 적용 함수
	UFUNCTION()
	void ApplyFireDamage();

	// 화염 데미지 종료 함수
	UFUNCTION()
	void StopFireDamage();

	



	UPROPERTY(EditAnywhere)
    class URespawnWidget* RespawnWidget;

	UFUNCTION()
	void UpdateRespawnUI();

public:
	
	FTimerHandle HPCooldownTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* arrowPositionComp;

	void OnMyActionArrow();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AArrowAttack> arrowFactory;

	// 화살을 쏘면 클릭한 위치에 화살이 가게끔

	float ArrowSpeed;

	UFUNCTION()
    void DelayedArrowAttack();

	UFUNCTION()
	void RotateToMouseDirection();

	FTimerHandle SkillTimerHandle;

	// 콤보 어택
	bool bComboAttacking;
	bool bComboAttackStart;
	bool bComboAttackNext;
	int comboAttackNumber;

	UFUNCTION(BlueprintCallable)
	void comboAttackStart();

	UFUNCTION(BlueprintCallable)
	void comboAttackEnd();

	UFUNCTION(BlueprintCallable)
	void comboAttack();

	UFUNCTION(BlueprintCallable)
	void comboAttackEndNotice();

	UFUNCTION(BlueprintCallable)
	void comboAttackCheck();

	//LSJ 인벤토리
	UPROPERTY(EditAnywhere)
	class UInventoryComponent* inventoryComponent;

	bool OnSkill;
	float characterSpeed;

	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void Respawn();

	UPROPERTY(EditDefaultsOnly)
	class AClickMovePlayerController* char_controller;
	void SetController(AClickMovePlayerController* cont);
	

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* swordMesh; 
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* bowMesh;
	
	void HideBowMesh();
	void ShowSwordMesh();

	bool bCanUseIncreaseHPItem = true;
	FTimerHandle CooldownTimerHandle;
	void ResetHPCooldown();
	bool bIsHPCooldownActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    UParticleSystem* ComboParticle;

	void SpawnComboParticle();
};
