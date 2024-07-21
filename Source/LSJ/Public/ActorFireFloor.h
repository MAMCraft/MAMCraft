// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ActorFireFloor.generated.h"

UCLASS()
class LSJ_API AActorFireFloor : public AActor
{
	GENERATED_BODY()
	float currentTime;
	UPROPERTY(VisibleDefaultsOnly, Category = "DESTROYTIME")
	float destroyTime = 2.0f;
	int explosionDamage;
	int fireDamage;
public:	
	// Sets default values for this actor's properties
	AActorFireFloor();
	void SetDamage(int eDamage, int fDamage);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, Category = "MESH")
	UStaticMeshComponent* mesh;

		UPROPERTY()
	class UGameplayStatics* GameStatic;
	UPROPERTY()
	class UParticleSystem * FireParticle;
	UPROPERTY()
	class UParticleSystem * ExplosionParticle;

		UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundWave> Sound;
};
