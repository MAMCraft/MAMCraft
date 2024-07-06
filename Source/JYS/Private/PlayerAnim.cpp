// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerCharacter.h"


UPlayerAnim::UPlayerAnim()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/GameResource/Player/AnimationEditTime/Montage/AM_AttackCombo.AM_AttackCombo'"));

	if (ATTACK_MONTAGE.Succeeded())
		attackComboMontage = ATTACK_MONTAGE.Object;
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	auto ownerPawn = TryGetPawnOwner();
	auto player = Cast<APlayerCharacter>(ownerPawn);

	// 캐스팅에 성공했다면
	if (player)
	{
		FVector velocity = player->GetVelocity();
		FVector forwardVector = player->GetActorForwardVector();
		MovementSpeed = FVector:: DotProduct(forwardVector, velocity);
	}


}

void UPlayerAnim::AnimNotify_NextAttackCheck()
{
	UE_LOG(LogTemp, Error, TEXT("AnimNotify_NextAttackCheck"));
	OnNextAttackCheck.Broadcast();
}

void UPlayerAnim::AnimNotify_EndAttack()
{
	UE_LOG(LogTemp, Error, TEXT("AnimNotify_EndAttack"));
	EndAttack.Broadcast();
}

FName UPlayerAnim::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

void UPlayerAnim::PlayAttackMontage()
{
	Montage_Play(attackComboMontage, 1.0f);
}

void UPlayerAnim::JumpToAttackMontageSection(int32 NewSection)
{
	PlayAttackMontage();
	UE_LOG(LogTemp, Error, TEXT("JumpToAttackMontageSection"));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), attackComboMontage);
}

void UPlayerAnim::JumpToAttackMontageSection1()
{
	PlayAttackMontage();
	Montage_JumpToSection(TEXT("Attack1"), attackComboMontage);
}

void UPlayerAnim::JumpToAttackMontageSection2()
{
	PlayAttackMontage();
	Montage_JumpToSection(TEXT("Attack2"), attackComboMontage);
}
