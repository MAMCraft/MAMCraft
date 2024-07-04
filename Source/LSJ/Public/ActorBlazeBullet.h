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
	bool one;
	bool takeExplosionDamage; //한번 폭발데미지를 받았는지
	float currentTime;
	UPROPERTY(VisibleDefaultsOnly, Category = "DESTROYTIME")
	float destroyTime = 2.0f;
public:	
	// Sets default values for this actor's properties
	AActorBlazeBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
//protected:
//	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
//	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
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

	UPROPERTY(EditAnywhere,Category=FIREFLOOR)
	TSubclassOf<AActorFireFloor> fireFloor;
	/*UFUNCTION()
	void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);
	*/
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
};
