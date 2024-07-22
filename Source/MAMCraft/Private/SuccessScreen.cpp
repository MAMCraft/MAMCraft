// Fill out your copyright notice in the Description page of Project Settings.


#include "SuccessScreen.h"
//#include "MediaPlayer.h"
//#include "MediaTexture.h"
//#include "MediaSource.h"
#include "Components/Image.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "MediaSource.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

void USuccessScreen::NativeConstruct()
{
	Super::NativeConstruct();

}
void USuccessScreen::StartButtonCallback()
{

	PlayVideo();
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(VideoPlayer->Slot);
	if (CanvasSlot)
	{
		CanvasSlot->SetZOrder(1);
	}
	GetWorld()->GetTimerManager().SetTimer(TimerHandle1, this, &USuccessScreen::TestFunction, 10.f);

}
void USuccessScreen::TestFunction()
{
	//PlayVideo();
	//UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(VideoPlayer->Slot);
	//if (CanvasSlot)
	//{
	//	CanvasSlot->SetZOrder(1);
	//}

	// 레벨을 바꿔주는 함수
	UGameplayStatics::OpenLevel(this, TEXT("Start"));
	this->SetVisibility(ESlateVisibility::Hidden);
}

void USuccessScreen::Test()
{
	// 레벨을 바꿔주는 함수
	UGameplayStatics::OpenLevel(this, TEXT("Start"));
	this->SetVisibility(ESlateVisibility::Hidden);
}


void USuccessScreen::PlayVideo()
{
	
	if (MediaPlayer && MediaSource)
	{
		if (MediaPlayer->CanPlaySource(MediaSource))
		{
			MediaPlayer->OpenSource(MediaSource);
			MediaPlayer->Play();
		}
	}
}
