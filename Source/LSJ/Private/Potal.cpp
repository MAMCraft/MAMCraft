// Fill out your copyright notice in the Description page of Project Settings.


#include "Potal.h"

// Sets default values
APotal::APotal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pBodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	UStaticMesh* pBodyMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'")).Object;
	pBodyMeshComp->SetStaticMesh(pBodyMesh);


	UMaterialInstance* mi = nullptr;
	ConstructorHelpers::FObjectFinder<UMaterialInstance> materialFinder(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/LSJ/Resource/MapResource/Potal/NewMaterial_Inst.NewMaterial_Inst'"));
	if (materialFinder.Succeeded())
	{
		mi = materialFinder.Object;
	}
	if (mi)
	{
		materialI = UMaterialInstanceDynamic::Create(mi, NULL);
		pBodyMeshComp->SetMaterial(0, materialI);
	}

}

// Called when the game starts or when spawned
void APotal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APotal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//materialI->SetScalarParameterValue(FName(TEXT("Param")), DeltaTime * 10.0f + 32.0f);
}

