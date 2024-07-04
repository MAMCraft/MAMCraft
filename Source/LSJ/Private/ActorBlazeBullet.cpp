// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorBlazeBullet.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

// Sets default values
AActorBlazeBullet::AActorBlazeBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = mesh;
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

	mesh->OnComponentHit.AddDynamic(this, &AActorBlazeBullet::OnHit);
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
	currentTime += DeltaTime;
	if (currentTime > destroyTime)
	{
		currentTime = 0;
		Destroy();
	}
}

void AActorBlazeBullet::SetAttacklocation(FVector location)
{
	attacklocation = location;

	FVector startLoc = GetActorLocation();      // �߻� ����
	FVector targetLoc = attacklocation;  // Ÿ�� ����.
	float arcValue = 0.5f;                       // ArcParam (0.0-1.0)
	outVelocity = FVector::ZeroVector;   // ��� Velocity
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), arcValue))
	{
		//FPredictProjectilePathParams predictParams(20.0f, startLoc, outVelocity, 15.0f);   // 20: tracing ������ ������Ÿ�� ũ��, 15: �ù����̼ǵǴ� Max �ð�(��)
		//predictParams.DrawDebugTime = 15.0f;     //����� ���� �������� �ð� (��)
		//predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime �� �����ϸ� EDrawDebugTrace::Type::ForDuration �ʿ�.
		//predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
		//FPredictProjectilePathResult result;
		//UGameplayStatics::PredictProjectilePath(this, predictParams, result);
		mesh->AddImpulse(outVelocity*100); // objectToSend�� �߻�ü * ���� ����ߵǴ°� ����.
	}
}

void AActorBlazeBullet::SetDamage(int eDamage, int fDamage)
{
	this->explosionDamage= eDamage;
	this->fireDamage = fDamage;
}

void AActorBlazeBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (one) //�ѹ� ��Ҵٸ� ����
		return;

	UE_LOG(LogTemp, Warning, TEXT("HitComp : %s"), *HitComp->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OtherActor : %s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OtherComp : %s"), *OtherComp->GetName());

	//�浹
	FCollisionQueryParams Params(NAME_None, true, this);
	FVector Center = Hit.Location;
	FHitResult HitResult;
	USTRUCT()
	TArray<FOverlapResult> OverlapResults;
	FVector attackExplosionBox = FVector(100, 100, 100); // ���߹���
	//FCollisionQueryParams Params(EName::Name, false, this);
	//����� ä�η� ��ȯ
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeBox(attackExplosionBox),
		Params
	);
	DrawDebugBox(GetWorld(), Center, attackExplosionBox, FColor::Blue, false, -1, 0, 20);
	if (bResult)
	{
		for (auto overlapResults : OverlapResults)
		{
			if (overlapResults.GetActor()->ActorHasTag("Player"))
			{
				FDamageEvent damageEvent;
				UE_LOG(LogTemp, Error, TEXT("AActorBlazeBullet OverlapMultiByChannel Damage : %s"), *overlapResults.GetActor()->GetName());
				//�������� �޴����� TakeDamage�� ȣ���Ѵ�
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
		//���� ���� ���� - �ݸ��� �ڽ�, ���� �ڽ� / 2�� �� �����/ ������ ó��
	}
	one = true;
}
