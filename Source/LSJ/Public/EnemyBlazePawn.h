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

	//�������̽� �Լ�(������ �����ϰ� �浹 On) 
	virtual void AttackHitCheck() override;
	//�������̽� �Լ�(������ ���� �浹 Off) 
	virtual void AttackHitEnd() override;
	//�������̽� �Լ�(���ݾִϸ��̼ǽ���) 
	virtual void Attack() override;
	//�������̽� �Լ�(���ݾִϸ��̼�����) 
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
