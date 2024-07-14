// Fill out your copyright notice in the Description page of Project Settings.


#include "MAMCGameModeBase.h"
#include "Blueprint/UserWidget.h"


void AMAMCGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (StartMenuClass)
    {
        StartMenu = CreateWidget<UUserWidget>(GetWorld(), StartMenuClass);
        if (StartMenu)
        {
            StartMenu->AddToViewport();
        }
    }
}
