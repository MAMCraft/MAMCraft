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
    TSubclassOf<AIncreaseHPItem> ItemToSpawn; // IncreaseHPItem 클래스 타입의 변수

    UFUNCTION()
    void OnChestClicked();



	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* TreasureMesh;

	// 애니메이션 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* OpenChestMontage;

private:
	UPROPERTY(VisibleAnywhere)
    class UBoxComponent* BoxComp;

	bool OnlyOnce;


	// 타이머 핸들
	FTimerHandle SpawnItemTimerHandle;

	// 아이템 스폰 함수
	void SpawnItemAfterDelay();
};
