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
		if (StatData)
		{
			level = StatData->level;
			hp = StatData->maxHp;
			attackDamage = StatData->attackDamage;
			moveSpeed = StatData->moveSpeed;
		}
	}

}

void UStatComponent::OnAttacked(float DamageAmount)
{
	hp -= DamageAmount;
	if (hp < 0)
		hp = 0;
}

