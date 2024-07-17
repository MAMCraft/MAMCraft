// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WarpGate.generated.h"

UCLASS()
class LSJ_API AWarpGate : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, Category = "MESH")
	UStaticMeshComponent* mesh;
	UPROPERTY(VisibleAnywhere, Category = "MESH")
	UBoxComponent* boxCollision;
public:	
	// Sets default values for this actor's properties
	AWarpGate();
	UPROPERTY(EditAnywhere)
	int state;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
