// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnWidget.h"
#include "Components/TextBlock.h"

void URespawnWidget::NativeConstruct()
{
	RespawnCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("RespawnCount")));
}

void URespawnWidget::SetRespawnCount(int32 CurrentRespawns, int32 MaxRespawns)
{
	if (RespawnCount)
	{
		RespawnCount->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CurrentRespawns, MaxRespawns)));
	}

	if (RespawnSlider)
	{
		float RespawnRatio = static_cast<int32>(CurrentRespawns) / (static_cast<float>(MaxRespawns)-1);
		RespawnSlider->SetValue(RespawnRatio);
	}
}
