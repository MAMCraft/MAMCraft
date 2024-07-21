#include "ComboAttack.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
// #include "ConstructorHelpers.h"

// Sets default values
AComboAttack::AComboAttack()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AComboAttack::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AComboAttack::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

