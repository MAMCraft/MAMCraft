// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "ActorFireFloor.h"
#include "ActorBlazeBullet.generated.h"

UCLASS()
class LSJ_API AActorBlazeBullet : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	FVector attacklocation;
	int explosionDamage;
	int fireDamage;
public:	
	// Sets default values for this actor's properties
	AActorBlazeBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere,Category=Collision)
	USphereComponent* collisionComp;
	UPROPERTY(VisibleAnywhere, Category = "MESH")
	UStaticMeshComponent* mesh;
	FVector outVelocity = FVector::ZeroVector;
	//발사체 형태의 물리 기능을 가진 컴포넌트
	//UPROPERTY(VisibleAnywhere, Category=Movement)
	//UProjectileMovementComponent* movementComp;
	UFUNCTION()
	void SetAttacklocation(FVector location);
		UFUNCTION()
	void SetDamage(int eDamage,int fDamage);
	UFUNCTION()
	void HitMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere,Category=FIREFLOOR)
	TSubclassOf<AActorFireFloor> fireFloor;
};
