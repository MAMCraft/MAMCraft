// Fill out your copyright notice in the Description page of Project Settings.


#include "FailScreen.h"

#include "Components/TextBlock.h"

void UFailScreen::SetDeathText(const FText& DeathText)
{
    if (DeathMessageTextBlock)
    {
        DeathMessageTextBlock->SetText(DeathText);
    }
}