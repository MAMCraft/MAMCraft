// Fill out your copyright notice in the Description page of Project Settings.


#include "HpBarWidget.h"
#include "StatComponent.h"
void UHpBarWidget::SetHPBar(float percent)
{
	hpPercent = percent;
	HPProgressBar->SetPercent(hpPercent);
}

float UHpBarWidget::GetHPBar()
{
	return HPProgressBar->GetPercent();
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
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	UpdateHPWidget();
}

void UHpBarWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
	if (HPProgressBar->GetPercent() >= 1.0f)
		HPProgressBar->SetVisibility(ESlateVisibility::Hidden);
	else
		HPProgressBar->SetVisibility(ESlateVisibility::Visible);
}