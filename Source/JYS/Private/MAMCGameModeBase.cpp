// Fill out your copyright notice in the Description page of Project Settings.


#include "MAMCGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "FailScreen.h"
#include "Kismet/GameplayStatics.h"
#include "OverScreen.h"



void AMAMCGameModeBase::BeginPlay()
{
    Super::BeginPlay();

}

AMAMCGameModeBase::AMAMCGameModeBase()
{

}


void AMAMCGameModeBase::showfailscreen()
{
	if (FailScreen != nullptr) {
		FailUI = CreateWidget<UFailScreen>(GetWorld(), FailScreen);
		if (FailUI != nullptr) {
			FailUI->AddToViewport();
		}
	}
}
void AMAMCGameModeBase::showoverscreen()
{
	if (OverScreen != nullptr) {
		OverUI = CreateWidget<UOverScreen>(GetWorld(), OverScreen);
		if (OverUI != nullptr) {
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			OverUI->AddToViewport();
		}
}
}



