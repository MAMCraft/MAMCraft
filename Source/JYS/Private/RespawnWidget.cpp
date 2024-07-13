// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnWidget.h"

void URespawnWidget::NativeConstruct()
{
}

void URespawnWidget::SetRespawnCount(int32 CurrentRespawns, int32 MaxRespawns)
{
	if (RespawnSlider)
	{
		float RespawnRatio = static_cast<int32>(CurrentRespawns) / static_cast<float>(MaxRespawns);
		RespawnSlider->SetValue(RespawnRatio);
	}
}
