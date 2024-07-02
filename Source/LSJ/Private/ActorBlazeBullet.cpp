// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorBlazeBullet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AActorBlazeBullet::AActorBlazeBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = collisionComp;
	mesh->SetupAttachment(collisionComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh>sphereMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (sphereMesh.Succeeded())
	{
		mesh->SetStaticMesh(sphereMesh.Object);
	}
	mesh->SetSimulatePhysics(true);

	static ConstructorHelpers::FClassFinder<AActorFireFloor> blazeAnim1(TEXT("/Script/Engine.Blueprint'/Game/LSJ/Blueprints/BP_FireFloor.BP_FireFloor_C'"));
	if (blazeAnim1.Succeeded())
	{
		fireFloor = (blazeAnim1.Class);
	}
}

// Called when the game starts or when spawned
void AActorBlazeBullet::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AActorBlazeBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (outVelocity != FVector::ZeroVector)
	{
		SetActorLocation(outVelocity);
	}

}

void AActorBlazeBullet::SetAttacklocation(FVector location)
{
	attacklocation = location;

	FVector startLoc = GetActorLocation();      // 발사 지점
	FVector targetLoc = attacklocation;  // 타겟 지점.
	float arcValue = 0.5f;                       // ArcParam (0.0-1.0)
	outVelocity = FVector::ZeroVector;   // 결과 Velocity
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), arcValue))
	{
		//FPredictProjectilePathParams predictParams(20.0f, startLoc, outVelocity, 15.0f);   // 20: tracing 보여질 프로젝타일 크기, 15: 시물레이션되는 Max 시간(초)
		//predictParams.DrawDebugTime = 15.0f;     //디버그 라인 보여지는 시간 (초)
		//predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime 을 지정하면 EDrawDebugTrace::Type::ForDuration 필요.
		//predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
		//FPredictProjectilePathResult result;
		//UGameplayStatics::PredictProjectilePath(this, predictParams, result);
		mesh->AddImpulse(outVelocity * 100.f); // objectToSend는 발사체 * 질량 해줘야되는거 같다.
	}
}

void AActorBlazeBullet::SetDamage(int eDamage, int fDamage)
{
	this->explosionDamage= eDamage;
	this->fireDamage = fDamage;
}

void AActorBlazeBullet::HitMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("explosionDamage %s"),*OtherActor->GetName());
	if (explosionDamage <= 0 && fireDamage <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("explosionDamage"));
		return;
	}
	if (OtherActor->ActorHasTag(FName("Map")))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		AActorFireFloor* fireInstance = Cast<AActorFireFloor>(GetWorld()->SpawnActor<AActorFireFloor>(fireFloor, OtherActor->GetActorLocation(), OtherActor->GetActorRotation(), SpawnParams));
		//fireInstance->SetDamage(statComponent->GetAttackDamage(), (statComponent->GetAttackDamage() / 10));
		//OtherActor->TakeDamage(statComponent->GetAttackDamage(), damageEvent, GetController(), this);
		//폭발 지역 생성 - 콜리전 박스, 빨간 박스 / 2초 뒤 사라짐/ 오버랩 처리
	}
}

void AActorBlazeBullet::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("explosionDamage %s"), *OtherActor->GetName());
	if (explosionDamage <= 0 && fireDamage <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("explosionDamage"));
		return;
	}
	if (OtherActor->ActorHasTag(FName("Map")))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		AActorFireFloor* fireInstance = Cast<AActorFireFloor>(GetWorld()->SpawnActor<AActorFireFloor>(fireFloor, OtherActor->GetActorLocation(), OtherActor->GetActorRotation(), SpawnParams));
		//fireInstance->SetDamage(statComponent->GetAttackDamage(), (statComponent->GetAttackDamage() / 10));
		//OtherActor->TakeDamage(statComponent->GetAttackDamage(), damageEvent, GetController(), this);
		//폭발 지역 생성 - 콜리전 박스, 빨간 박스 / 2초 뒤 사라짐/ 오버랩 처리
	}
}
