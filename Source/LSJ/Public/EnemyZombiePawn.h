// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAnimationAttackInterface.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"
#include "AnimInstanceZombie.h"
#include "StatComponent.h"
#include "EnemyZombiePawn.generated.h"


UCLASS()
class LSJ_API AEnemyZombiePawn : public APawn, public IEnemyAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyZombiePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	//인터페이스 함수(공격이 가능하게 충돌 On) - 추후에 부모 EnemyPawn에 넣어줘야함
	virtual void AttackHitCheck() override;
	//인터페이스 함수(공격이 끝남 충돌 Off) - 추후에 부모 EnemyPawn에 넣어줘야함
	virtual void AttackHitEnd() override;
	//인터페이스 함수(공격애니메이션시작) - 추후에 부모 EnemyPawn에 넣어줘야함
	virtual void Attack() override;
	//인터페이스 함수(공격애니메이션종료) - 추후에 부모 EnemyPawn에 넣어줘야함
	virtual void OnAttackEnd() override;
	//데미지 받는 함수
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "MESH")
	USkeletalMeshComponent* skMeshComponent;
	UPROPERTY(VisibleAnywhere, Category = "COLLISIONCAPCULE")
	UCapsuleComponent* capsuleComponent;
	UPROPERTY(VisibleAnywhere, Category = "ROOT")
	USceneComponent* rootComp;
	UPROPERTY(VisibleAnywhere, Category = "ROOT")
	FVector rootCompScale;
	UPROPERTY(VisibleAnywhere, Category = "MOVEMENT")
	UFloatingPawnMovement* movement;

	UPROPERTY(VisibleAnywhere, Category = "COLLISIONARM")
	UBoxComponent* handAttackComponent;

	UAnimInstanceZombie* animInstance=nullptr;

	UPROPERTY(VisibleAnywhere)
	class UStatComponent* statComponent;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
