// Fill out your copyright notice in the Description page of Project Settings.


#include "HPWidget.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetSystemLibrary.h"

void UHPWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHPWidget::SetHP(int32 hp, int32 maxHP)
{
	PlayerHP->SetPercent((float)hp / maxHP);
}

