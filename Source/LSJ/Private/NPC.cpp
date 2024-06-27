// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceFind(TEXT("/Game/LSJ/Animation/BPZombieAnim.BPZombieAnim_C"));
	if (AnimInstanceFind.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(AnimInstanceFind.Class);
	}
	//AnimInstance == nullptr
	AnimInstance = GetMesh()->GetAnimInstance();
	UE_LOG(LogTemp, Display, TEXT("AnimInstance==nullptr %s"), AnimInstance==nullptr?TEXT("true") : TEXT("false"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageFind(TEXT("/Script/Engine.AnimMontage'/Game/LSJ/Animation/Attack/Zombie_Attack_Anim_mixamo_com1_Montage.Zombie_Attack_Anim_mixamo_com1_Montage'"));
	if (AttackMontageFind.Succeeded()) {
		AttackMontage = AttackMontageFind.Object;
	}
}

UBehaviorTree* ANPC::GetBehaviorTree() const
{
	
	return Tree;
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC::Attack()
{
	if (isAttacking)
		return;

	AnimInstance ->Montage_Play(AttackMontage);
	UE_LOG(LogTemp, Display, TEXT("AttackMontage"));
	isAttacking = true;
}
// 애니메이션 이벤트
void ANPC::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruped)
{
	UE_LOG(LogTemp, Display, TEXT("?isAttacking? %s"), isAttacking? TEXT("true") : TEXT("false"));
	isAttacking = false;
	OnAttackEnd.Broadcast();		//AttackEnd의 BroadCast호출
}
