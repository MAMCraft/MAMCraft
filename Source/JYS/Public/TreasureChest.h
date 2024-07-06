// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IncreaseHPItem.h"
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

private:
    class UBoxComponent* BoxComp;
    class UStaticMeshComponent* ChestMesh;
};
