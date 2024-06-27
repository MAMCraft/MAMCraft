// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "NPC_AIController.h"
#include "NPC.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	//ƽ��� Ȱ��ȭ. Finish�� ��� üũ�Ѵ�.
	bNotifyTick = true;
	//���� �������� �ƴϴ�
	isAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ANPC* npc = Cast<ANPC>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == npc)
	{
		UE_LOG(LogTemp, Display, TEXT("nullptr == npc"));
		return EBTNodeResult::Failed;
	}
	npc->Attack();
	auto Blackboard = OwnerComp.GetBlackboardComponent();
	Blackboard->SetValueAsBool(FName("IsAttacking"), true);
	isAttacking = true;
	//���ٽ�.ABCharacter�� AttackEnd Delegate�� ȣ���ϸ� IsAttacking�� false��
	npc->OnAttackEnd.AddLambda([this]()->void {
		isAttacking = false;
		});

	//�ϴ� InProgress���� �ӹ����Ѵ�.������ ������ ������ ��� ���������ش�.
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	//OnAttackEnd Delegate�� ȣ��Ǹ� Attacking�� false �ǹǷ�
	if (!isAttacking)
	{
		auto Blackboard = OwnerComp.GetBlackboardComponent();
		Blackboard->SetValueAsBool(FName("IsAttacking"), false);
		//���� �½�ũ�� �������� �˷��ش�
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
