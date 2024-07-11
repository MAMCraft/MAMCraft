#include "ComboAttack.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
// #include "ConstructorHelpers.h"

// Sets default values
AComboAttack::AComboAttack()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //// 칼 메시 컴포넌트를 초기화하지 않고 null로 설정
    //SwordMesh = nullptr;

    //// 소켓 이름 설정
    //SocketName = FName("R_Arm_armor_end");
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

//void AComboAttack::StartComboAttack()
//{
//    // 부모 액터의 메시 컴포넌트를 가져옵니다.
//    USkeletalMeshComponent* ParentMesh = Cast<USkeletalMeshComponent>(GetAttachParentActor()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
//    if (ParentMesh && ParentMesh->DoesSocketExist(SocketName))
//    {
//        // 칼 메시 컴포넌트를 생성하고 소켓에 부착합니다.
//        SwordMesh = NewObject<UStaticMeshComponent>(this, TEXT("SwordMesh"));
//        SwordMesh->RegisterComponent();
//        SwordMesh->AttachToComponent(ParentMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
//
//        // 칼 메시를 설정합니다.
//        ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
//        if (TempMesh.Succeeded())
//        {
//            SwordMesh->SetStaticMesh(TempMesh.Object);
//            SwordMesh->SetRelativeScale3D(FVector(0.75f, 0.25f, 1));
//        }
//    }
//}
//
//void AComboAttack::EndComboAttack()
//{
//    // 칼 메시 컴포넌트를 제거합니다.
//    if (SwordMesh)
//    {
//        SwordMesh->DestroyComponent();
//        SwordMesh = nullptr;
//    }
//}
