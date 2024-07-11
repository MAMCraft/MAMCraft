#include "ComboAttack.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
// #include "ConstructorHelpers.h"

// Sets default values
AComboAttack::AComboAttack()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //// Į �޽� ������Ʈ�� �ʱ�ȭ���� �ʰ� null�� ����
    //SwordMesh = nullptr;

    //// ���� �̸� ����
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
//    // �θ� ������ �޽� ������Ʈ�� �����ɴϴ�.
//    USkeletalMeshComponent* ParentMesh = Cast<USkeletalMeshComponent>(GetAttachParentActor()->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
//    if (ParentMesh && ParentMesh->DoesSocketExist(SocketName))
//    {
//        // Į �޽� ������Ʈ�� �����ϰ� ���Ͽ� �����մϴ�.
//        SwordMesh = NewObject<UStaticMeshComponent>(this, TEXT("SwordMesh"));
//        SwordMesh->RegisterComponent();
//        SwordMesh->AttachToComponent(ParentMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
//
//        // Į �޽ø� �����մϴ�.
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
//    // Į �޽� ������Ʈ�� �����մϴ�.
//    if (SwordMesh)
//    {
//        SwordMesh->DestroyComponent();
//        SwordMesh = nullptr;
//    }
//}
