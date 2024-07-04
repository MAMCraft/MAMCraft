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

	float hitKnockbackTime=0.8f;
	float hitTime=0.5f;
	float currentTime;
	bool bHit;
	void Hit(AActor* damageCauser);
	FVector outVelocity;
	float groundZValue;
	UBlackboardComponent* bc;
	FVector pawnLocation;
	FVector direction;
public:
	// Sets default values for this pawn's properties
	AEnemyZombiePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	virtual void Attack(TArray<FVector>& location) override;
	//�������̽� �Լ�(������ �����ϰ� �浹 On)
	virtual void AttackHitCheck() override;
	//�������̽� �Լ�(������ ���� �浹 Off)
	virtual void AttackHitEnd() override;
	//�������̽� �Լ�(���ݾִϸ��̼ǽ���) 
	virtual void Attack() override;
	//�������̽� �Լ�(���ݾִϸ��̼�����) 
	virtual void OnAttackEnd() override;
	//������ �޴� �Լ�
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "MESH")
	USkeletalMeshComponent* skMeshComponent;
	UPROPERTY(VisibleAnywhere, Category = "COLLISIONCAPCULE")
	UCapsuleComponent* capsuleComponent;
	//UPROPERTY(VisibleAnywhere, Category = "ROOT")
	//USceneComponent* rootComp;
	//UPROPERTY(VisibleAnywhere, Category = "ROOT")
	//FVector rootCompScale;
	UPROPERTY(VisibleAnywhere, Category = "MOVEMENT")
	UFloatingPawnMovement* movement;

	UPROPERTY(VisibleAnywhere, Category = "COLLISIONARM")
	UBoxComponent* handAttackComponent;

	UAnimInstanceZombie* animInstance=nullptr;

	UPROPERTY(VisibleAnywhere)
	class UStatComponent* statComponent;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HitEnd();
};
