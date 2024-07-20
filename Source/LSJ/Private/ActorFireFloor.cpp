
// Fill out your copyright notice in the Description page of Project Settings.
#include "ActorFireFloor.h"
#include "Engine/DamageEvents.h"


// Sets default values
AActorFireFloor::AActorFireFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = mesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh>boxMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (boxMesh.Succeeded())
	{
		mesh->SetStaticMesh(boxMesh.Object);
	}


	//mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), ChangeColor);

		//Effect
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("/Script/Engine.ParticleSystem'/Game/M5VFXVOL2/Particles/Fire/Fire_02.Fire_02'"));
	if (Fire.Succeeded())
	{
		FireParticle = Fire.Object;
	}
	//Effect
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Explosion(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Gruntling/Bomber/P_FireBombExplosion.P_FireBombExplosion'"));
	if (Explosion.Succeeded())
	{
		ExplosionParticle = Explosion.Object;
	}
	mesh->OnComponentBeginOverlap.AddDynamic(this,&AActorFireFloor::OverlapBegin);
	mesh->OnComponentEndOverlap.AddDynamic(this,&AActorFireFloor::OnOverlapEnd);
}
void AActorFireFloor::SetDamage(int eDamage, int fDamage)
{
	this->explosionDamage = eDamage;
	this->fireDamage = fDamage;
}
// Called when the game starts or when spawned
void AActorFireFloor::BeginPlay()
{
	Super::BeginPlay();
	mesh->SetVisibility(false);
	GameStatic->SpawnEmitterAttached(FireParticle, RootComponent, FName("Muzzle"));
	//이펙트
	GameStatic->SpawnEmitterAttached(ExplosionParticle, RootComponent);
}

// Called every frame
void AActorFireFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentTime += DeltaTime;
	if (currentTime > destroyTime)
	{
		currentTime = 0;
		Destroy();
	}
}

void AActorFireFloor::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		FDamageEvent damageEvent;
		//데미지를 받는쪽의 TakeDamage를 호출한다
		OtherActor->TakeDamage(explosionDamage, damageEvent, nullptr, this);

		//지속 데미지
		//FDamageEvent damageEvent;
		// 데미지를 받는쪽의 TakeDamage를 호출한다
		//OtherActor->TakeDamage(statComponent->GetAttackDamage(), damageEvent, GetController(), this);
		//델리게이트
		//OtherActor->OnTakeAnyDamage();
	}
}

void AActorFireFloor::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		FDamageEvent damageEvent;
		//데미지를 받는쪽의 TakeDamage를 호출한다
		OtherActor->TakeDamage(explosionDamage, damageEvent, nullptr, this);

		//지속 데미지
		//FDamageEvent damageEvent;
		//UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());
		// 데미지를 받는쪽의 TakeDamage를 호출한다
		//OtherActor->TakeDamage(statComponent->GetAttackDamage(), damageEvent, GetController(), this);
		//델리게이트
		//OtherActor->OnTakeAnyDamage();
	}
}

