// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MAMCGameInstance.h"
// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false; // ƽ�� ���� �ʿ����

	bWantsInitializeComponent = true; // InitializeComponent �Լ� ȣ���� ���� true
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
	// GameInstance�� �޾ƿͼ� ���� ���� ������
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
		//Clamp�� �ּҼ�ġ�� �ִ� ��ġ�� ���� �� ���̿����� ����� ������ �ϴ� �Լ���.
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
	//ü�� ���� ��������Ʈ ȣ��
	OnHPChanged.Broadcast();
	// float��ġ�� 0�� �ִ��� ������ ���� �����ϰ��
	if (currentHp < 1)
	{
		currentHp = 0;

		//�׾������� ��������Ʈ ȣ��
		OnHPIsZero.Broadcast();
	}
}
