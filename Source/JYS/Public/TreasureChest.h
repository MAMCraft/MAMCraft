// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IncreaseHPItem.h"
#include "Animation/AnimMontage.h"
#include "TreasureChest.generated.h"

UCLASS()
class JYS_API ATreasureChest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATreasureChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly, Category = "Items")
    TSubclassOf<AIncreaseHPItem> ItemToSpawn; // IncreaseHPItem Ŭ���� Ÿ���� ����

    UFUNCTION()
    void OnChestClicked();



	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* TreasureMesh;

	// �ִϸ��̼� ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* OpenChestMontage;

private:
	UPROPERTY(VisibleAnywhere)
    class UBoxComponent* BoxComp;

	bool OnlyOnce;


	// Ÿ�̸� �ڵ�
	FTimerHandle SpawnItemTimerHandle;

	// ������ ���� �Լ�
	void SpawnItemAfterDelay();
};
