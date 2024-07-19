// Fill out your copyright notice in the Description page of Project Settings.


#include "UIDamage.h"

void UUIDamage::SetDamageText(int amount)
{
	FString FormattedNum = FString::Printf(TEXT("%d"), amount);
	DamageText->SetText(FText::FromString(FormattedNum));
}

int UUIDamage::GetDamageText()
{
	return FCString::Atoi(*DamageText->GetText().ToString());
}

void UUIDamage::PlayAnimationDamageUI()
{
	damageWidgetAnimationEvent.Clear();
	damageWidgetAnimationEvent.BindUFunction(this, FName(FString(TEXT("OnEndPlayAnimation"))));
	BindToAnimationFinished(DamageOpacity, damageWidgetAnimationEvent);
	PlayAnimation(DamageOpacity);
}

void UUIDamage::HiddenText()
{
	DamageText->SetVisibility(ESlateVisibility::Hidden);
}

void UUIDamage::VisibleText()
{
	DamageText->SetVisibility(ESlateVisibility::Visible);
}

void UUIDamage::OnEndPlayAnimation()
{
	
}
