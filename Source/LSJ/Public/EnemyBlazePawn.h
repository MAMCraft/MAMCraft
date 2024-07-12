// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAnimationAttackInterface.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"
#include "AnimInstanceBlaze.h"
#include "ActorBlazeBullet.h"
#include "StatComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HpBarWidget.h"
#include "Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "EnemyBlazePawn.generated.h"

UCLASS()
class LSJ_API AEnemyBlazePawn : public APawn, public IEnemyAnimationAttackInterface
{
	GENERATED_BODY()

	float hitTime = 0.3f;
	float hitCurrentTime;
	bool bHit;
	void Hit(AActor* damageCauser);

	float dieDestroyTime = 2.f;
	float dieFalldownTime = 1.5f;
	float dieCurrentTime;
	bool bDie;
	FVector hitDirection;
	FVector dieDirection;
	void Die(AActor* damageCauser);
	UBlackboardComponent* bc;

	float MisSpeed;


	//#region 속도 변수들
	double velocityDie = 30.f;
	float FloatVelo = 0.f;
	float gravityDie = 09.82f;
	float Rad = 75.f;

	float MaxH = 0.f;

	float PositionX = 0.f;
	float PositionY = 0.f;

	float timeDie = 0.f;

	double Velo_x = 0.0;
	double Velo_y = 0.0;
	bool SimulationStart = false;

	float fx = 0.f;
	float fy = 0.f;

	float groundZValue;
public:
	// Sets default values for this pawn's properties
	AEnemyBlazePawn();

	int attackRangeindex=0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess="true"))
	class UBehaviorTree* bTree;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI", meta=(AllowPrivateAccess="true"))
	class UBlackboardData* bData;

	
public:	
	UBehaviorTree* GetBehaviorTree() const;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//인터페이스 함수(공격이 가능하게 충돌 On) 
	virtual void AttackHitCheck() override;
	//인터페이스 함수(공격이 끝남 충돌 Off) 
	virtual void AttackHitEnd() override;
	//인터페이스 함수(공격애니메이션시작) 
	virtual void Attack() override;
	//인터페이스 함수(공격애니메이션종료) 
	virtual void OnAttackEnd() override;
	//인터페이스 함수(공격애니메이션시작 - blaze) 
	virtual void Attack(TArray<FVector>& location) override;
	//데미지 받는 함수
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void Fire();

	UPROPERTY(VisibleAnywhere, Category = "MESH")
	USkeletalMeshComponent* skMeshComponent;
	UPROPERTY(VisibleAnywhere, Category = "COLLISIONCAPCULE")
	UBoxComponent* boxComponent;
	UPROPERTY(VisibleAnywhere, Category = "MOVEMENT")
	UFloatingPawnMovement* movement;
	UAnimInstanceBlaze* animInstance = nullptr;
	UPROPERTY(EditAnywhere,Category=BULLET)
	TSubclassOf<class AActorBlazeBullet> bullet;
	TArray<FVector> attackLocation;

	UPROPERTY(VisibleAnywhere)
	class UStatComponent* statComponent;

	//hit material
	UMaterialInterface* hitMaterial;
	UMaterialInterface* originMaterial;
	FTimerHandle damageBlinkTimerHandle;
	void BlinkRed();
	void EndBlink();
	//HPBar
	UPROPERTY(VisibleAnyWhere, Category = UI)
	class UWidgetComponent* hpBarWidgetComponent;
	UHpBarWidget* hpBarWidget;
	//Effect
	UPROPERTY()
	class UGameplayStatics* GameStatic;
	UPROPERTY()
	class UParticleSystem * FireParticle;
};
