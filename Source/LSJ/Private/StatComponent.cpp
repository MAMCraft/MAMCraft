// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MAMCGameInstance.h"
// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false; // 틱이 따로 필요없음

	bWantsInitializeComponent = true; // InitializeComponent 함수 호출을 위해 true
	level = 0;

	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevel(FName("Zombie"));
}

void UStatComponent::SetLevel(FName name)
{
	// GameInstance를 받아와서 안의 값을 가져옴
	auto gameInstance = Cast<UMAMCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (gameInstance)
	{
		auto StatData = gameInstance->GetStatData(name);
		{
			level = StatData.level;
			hp = StatData.maxHp;
			attackDamage = StatData.attackDamage;
			moveSpeed = StatData.moveSpeed;
			currentHp = hp;
		}
	}

}

void UStatComponent::OnAttacked(float DamageAmount)
{
	hp -= DamageAmount;
	if (hp < 0)
		hp = 0;
}

float UStatComponent::GetHPRatio()
{
	return (float)currentHp / hp;
}

void UStatComponent::SetDamage(int NewDamage)
{
	//if (CurrentStatData != nullptr)
	{
		//Clamp는 최소수치와 최대 수치를 정해 그 사이에서만 결과가 나오게 하는 함수다.
		/*CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0, CurrentStatData->MaxHP);
		if (CurrentHP <= 0)
		{
			OnHPIsZero.Broadcast();
		}*/
		//SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0, CurrentStatData->MaxHP));
		SetHP(FMath::Clamp<float>(currentHp - NewDamage, 0, hp));
	}
}

void UStatComponent::SetHP(int NewHP)
{
	currentHp = NewHP;
	//체력 변동 딜리게이트 호출
	OnHPChanged.Broadcast();
	// float수치로 0에 최대한 근접한 오차 이하일경우
	if (currentHp < 1)
	{
		currentHp = 0;

		//죽었을떄의 딜리게이트 호출
		OnHPIsZero.Broadcast();
	}
}
