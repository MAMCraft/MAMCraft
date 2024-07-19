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
            UE_LOG(LogTemp, Warning, TEXT("ddddd"));
            PlayerCharacter->rightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            PlayerCharacter->rightWeaponCollision->SetGenerateOverlapEvents(true);  // 충돌 이벤트를 생성하도록 설정
        }
    }
}

void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()))
    {
        if (PlayerCharacter->rightWeaponCollision)
        {
            UE_LOG(LogTemp, Warning, TEXT("fffff"));
            PlayerCharacter->rightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            PlayerCharacter->rightWeaponCollision->SetGenerateOverlapEvents(false);  // 충돌 이벤트를 비활성화
        }
    }
}
