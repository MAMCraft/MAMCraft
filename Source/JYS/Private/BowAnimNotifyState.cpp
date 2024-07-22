// Fill out your copyright notice in the Description page of Project Settings.


#include "BowAnimNotifyState.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBowAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()))
    {
        if (PlayerCharacter->rightWeaponCollision)
        {
            //UE_LOG(LogTemp, Warning, TEXT("ddddd"));
            //PlayerCharacter->rightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            //PlayerCharacter->rightWeaponCollision->SetGenerateOverlapEvents(true);  // �浹 �̺�Ʈ�� �����ϵ��� ����
            PlayerCharacter->OnSkill = true;
            PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 0;
            PlayerCharacter->bowMesh->SetVisibility(true);
            PlayerCharacter->swordMesh->SetVisibility(false);
        }
    }
}

void UBowAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()))
    {
        if (PlayerCharacter->rightWeaponCollision)
        {
            //UE_LOG(LogTemp, Warning, TEXT("fffff"));
            //PlayerCharacter->rightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            //PlayerCharacter->rightWeaponCollision->SetGenerateOverlapEvents(false);  // �浹 �̺�Ʈ�� ��Ȱ��ȭ
            PlayerCharacter->OnSkill = false;
            PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = PlayerCharacter->characterSpeed;
            PlayerCharacter->AnimInstance->Montage_Stop(1.0f);
            PlayerCharacter->HideBowMesh();
            PlayerCharacter->ShowSwordMesh();
        }
    }
}
