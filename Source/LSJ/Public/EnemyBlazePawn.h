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
#include "EnemyBlazePawn.generated.h"

UCLASS()
class LSJ_API AEnemyBlazePawn : public APawn, public IEnemyAnimationAttackInterface
{
	GENERATED_BODY()

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
	virtual void Attack(TArray<FVector>& location) override;
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
};
