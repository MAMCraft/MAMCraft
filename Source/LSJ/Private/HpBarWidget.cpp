// Fill out your copyright notice in the Description page of Project Settings.


#include "HpBarWidget.h"
#include "StatComponent.h"
void UHpBarWidget::SetHPBar(float percent)
{
	hpPercent = percent;
	HPBar->SetPercent(hpPercent);
}

float UHpBarWidget::GetHPBar()
{
	return HPBar->GetPercent();
}

void UHpBarWidget::BindCharacterStat(UStatComponent* NewCharacterStat)
{
	if (nullptr != NewCharacterStat)
	{
		CurrentCharacterStat = NewCharacterStat;
		NewCharacterStat->OnHPChanged.AddUObject(this, &UHpBarWidget::UpdateHPWidget);
		//SetHPBar(NewCharacterStat->GetHp() / NewCharacterStat->GetMaxHp());
	}
}


void UHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));

	UpdateHPWidget();
}

void UHpBarWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPBar)
		{
			HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
	if (HPBar->GetPercent() >= 1.0f || HPBar->GetPercent()<=0.0f)
		HPBar->SetVisibility(ESlateVisibility::Hidden);
	else
		HPBar->SetVisibility(ESlateVisibility::Visible);
}