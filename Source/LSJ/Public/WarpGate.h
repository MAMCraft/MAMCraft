// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarpGate.generated.h"

UCLASS()
class LSJ_API AWarpGate : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, Category = "MESH")
	UStaticMeshComponent* mesh;
public:	
	// Sets default values for this actor's properties
	AWarpGate();
	UPROPERTY(EditAnywhere)
	int state;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	UFUNCTION()
	virtual void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
