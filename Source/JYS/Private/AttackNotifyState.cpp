// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackNotifyState.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"

void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()))
    {
        if (PlayerCharacter->rightWeaponCollision)
        {
            PlayerCharacter->rightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        }
    }
}

void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()))
    {
        if (PlayerCharacter->rightWeaponCollision)
        {
            PlayerCharacter->rightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}
