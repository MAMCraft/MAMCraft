// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorBlazeBullet.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AActorBlazeBullet::AActorBlazeBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = root;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	mesh->SetupAttachment(RootComponent);
	//RootComponent = mesh;

	ConstructorHelpers::FObjectFinder<UStaticMesh>sphereMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (sphereMesh.Succeeded())
	{
		mesh->SetStaticMesh(sphereMesh.Object);
	}
	mesh->SetSimulatePhysics(true);

	static ConstructorHelpers::FClassFinder<AActorFireFloor> fireFloorBP(TEXT("/Script/Engine.Blueprint'/Game/LSJ/Blueprints/BP_FireFloor.BP_FireFloor_C'"));
	if (fireFloorBP.Succeeded())
	{
		fireFloor = (fireFloorBP.Class);
	}
	//Effect UNiagaraSystem
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ParticleAsset1(TEXT("/Script/Niagara.NiagaraSystem'/Game/sA_Megapack_v1/sA_ShootingVfxPack/FX/NiagaraSystems/NS_ROCKET_Trail.NS_ROCKET_Trail'"));
	if (ParticleAsset1.Succeeded())
	{
		FireEffectMuzzle = ParticleAsset1.Object;
	}
	//Effect UParticleSystemComponent
	//particleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	//particleComponent->SetupAttachment(RootComponent);
	//particleComponent->bAutoActivate = false;
	//particleComponent->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));

	//		static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/sA_Megapack_v1/sA_ShootingVfxPack/FX/NiagaraSystems/NS_ROCKET_Trail.NS_ROCKET_Trail'"));
	//if (ParticleAsset.Succeeded())
	//{particleComponent->SetTemplate(ParticleAsset.Object);
	//	particleComponent->SetTemplate(FireParticle);
	//}

	mesh->OnComponentHit.AddDynamic(this, &AActorBlazeBullet::OnHit);
}

// Called when the game starts or when spawned
void AActorBlazeBullet::BeginPlay()
{
	Super::BeginPlay();
	mesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	//Effect UParticleSystemComponent
	//particleComponent->ToggleActive();
	mesh->SetVisibility(false);
	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(FireEffectMuzzle, mesh, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
	//회전
	//파티클 변수로 저장해서 TICK에서 회전
	//옷 가져가기
}

// Called every frame
void AActorBlazeBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentTime += DeltaTime;
	if (currentTime > destroyTime)
	{
		currentTime = 0;
		Destroy();
	}
	//rotation
	//mesh->SetRelativeRotation(outVelocity.Rotation());
	/*FVector angularVelocityDelta = mesh->GetPhysicsAngularVelocityInDegrees();
	FVector forwardVector = angularVelocityDelta.ToOrientationQuat().GetForwardVector();
	FVector rotate = FVector::CrossProduct(angularVelocityDelta, forwardVector);*/
	//particleComponent->SetRelativeRotation(outVelocity.Rotation());
	//SetActorRelativeRotation(outVelocity.Rotation());
}

void AActorBlazeBullet::SetAttacklocation(FVector location)
{
	attacklocation = location;

	FVector startLoc = GetActorLocation();      // 발사 지점
	startLoc.Z += 80.f;
	FVector targetLoc = attacklocation;  // 타겟 지점.
	float arcValue = 0.99f;                       // ArcParam (0.0-1.0)
	outVelocity = FVector::ZeroVector;   // 결과 Velocity
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), arcValue)) //GetWorld()->GetGravityZ()
	{
		SetActorRelativeRotation(outVelocity.Rotation());
		//FPredictProjectilePathParams predictParams(20.0f, startLoc, outVelocity, 15.0f);   // 20: tracing 보여질 프로젝타일 크기, 15: 시물레이션되는 Max 시간(초)
		//predictParams.DrawDebugTime = 15.0f;     //디버그 라인 보여지는 시간 (초)
		//predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime 을 지정하면 EDrawDebugTrace::Type::ForDuration 필요.
		//predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
		//FPredictProjectilePathResult result;
		//UGameplayStatics::PredictProjectilePath(this, predictParams, result);
		mesh->AddImpulse(outVelocity*20.0f); // objectToSend는 발사체 * 질량 해줘야되는거 같다.
		
	}
}

void AActorBlazeBullet::SetDamage(int eDamage, int fDamage)
{
	this->explosionDamage= eDamage;
	this->fireDamage = fDamage;
}

void AActorBlazeBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (one) //한번 닿았다면 리턴
		return;

	UE_LOG(LogTemp, Warning, TEXT("HitComp : %s"), *HitComp->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OtherActor : %s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OtherComp : %s"), *OtherComp->GetName());

	//충돌
	FCollisionQueryParams Params(NAME_None, true, this);
	FVector Center = Hit.Location;
	FHitResult HitResult;
	USTRUCT()
	TArray<FOverlapResult> OverlapResults;
	FVector attackExplosionBox = FVector(100, 100, 100); // 폭발범위
	//FCollisionQueryParams Params(EName::Name, false, this);
	//결과를 채널로 반환
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeBox(attackExplosionBox),
		Params
	);
	//DrawDebugBox(GetWorld(), Center, attackExplosionBox, FColor::Blue, false, -1, 0, 20);
	if (bResult)
	{
		for (auto overlapResults : OverlapResults)
		{
			if (overlapResults.GetActor()->ActorHasTag("Player"))
			{
				FDamageEvent damageEvent;
				UE_LOG(LogTemp, Error, TEXT("AActorBlazeBullet OverlapMultiByChannel Damage : %s"), *overlapResults.GetActor()->GetName());
				//데미지를 받는쪽의 TakeDamage를 호출한다
				overlapResults.GetActor()->TakeDamage(explosionDamage, damageEvent, nullptr, this);
				break;
			}
		}
	}

	//FireFloor spawn
	if (OtherActor->ActorHasTag(FName("Floor")))
	{
		if (explosionDamage <= 0 && fireDamage <= 0)
		{
			UE_LOG(LogTemp, Log, TEXT("OnHit fail"));
			return;
		}
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		AActorFireFloor* fireInstance = Cast<AActorFireFloor>(GetWorld()->SpawnActor<AActorFireFloor>(fireFloor, Hit.Location, OtherActor->GetActorRotation(), SpawnParams));
		fireInstance->SetDamage(explosionDamage, fireDamage);

		Destroy();
		/*FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		AActorFireFloor* fireInstance = Cast<AActorFireFloor>(GetWorld()->SpawnActor<AActorFireFloor>(fireFloor, Hit.Location, OtherActor->GetActorRotation(), SpawnParams));
	*/	//fireInstance->SetDamage(statComponent->GetAttackDamage(), (statComponent->GetAttackDamage() / 10));
		//OtherActor->TakeDamage(statComponent->GetAttackDamage(), damageEvent, GetController(), this);
		//폭발 지역 생성 - 콜리전 박스, 빨간 박스 / 2초 뒤 사라짐/ 오버랩 처리
	}
	one = true;
}
