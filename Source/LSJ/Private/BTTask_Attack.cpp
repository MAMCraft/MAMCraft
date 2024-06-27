// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "NPC_AIController.h"
#include "NPC.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	//틱기능 활성화. Finish를 계속 체크한다.
	bNotifyTick = true;
	//아직 공격중이 아니다
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
	//람다식.ABCharacter이 AttackEnd Delegate를 호출하면 IsAttacking을 false로
	npc->OnAttackEnd.AddLambda([this]()->void {
		isAttacking = false;
		});

	//일단 InProgress에서 머물게한다.공격이 끝나기 전까지 계속 지연시켜준다.
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	//OnAttackEnd Delegate가 호출되면 Attacking이 false 되므로
	if (!isAttacking)
	{
		auto Blackboard = OwnerComp.GetBlackboardComponent();
		Blackboard->SetValueAsBool(FName("IsAttacking"), false);
		//공격 태스크가 끝났음을 알려준다
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
