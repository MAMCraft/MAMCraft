// Fill out your copyright notice in the Description page of Project Settings.


#include "UIStartMenu.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "MediaSource.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

	// Start ��ư Ŭ���� ����� �Լ�
void UUIStartMenu::StartButtonCallback()
{
	PlayVideo();
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(VideoPlayer->Slot);
	if (CanvasSlot)
	{
		CanvasSlot->SetZOrder(1);
	}
	UGameplayStatics::PlaySound2D(GetWorld(), MySoundCue);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UUIStartMenu::TestFunction, 15.f);

}
void UUIStartMenu::TestFunction()
{
	// ������ �ٲ��ִ� �Լ�
	UGameplayStatics::OpenLevel(this, TEXT("Forest"));
	OnButtonLevelUpdated.Broadcast();
	this->SetVisibility(ESlateVisibility::Hidden);
}
void UUIStartMenu::PlayVideo()
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

// End ��ư Ŭ���� ����� �Լ�
void UUIStartMenu::EndButtonCallback()
{
	// ���� ���� �Լ�
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
	//RemoveFromParent();
}

void UUIStartMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� �������Ʈ�� ��ư�� �̸��� ���� �����´�
	//Start = Cast<UButton>(GetWidgetFromName(TEXT("Start")));
	//Exit = Cast<UButton>(GetWidgetFromName(TEXT("End")));

	// ��ư Ŭ���� ȣ��� ��������Ʈ�� �Լ��� ����Ѵ�
	Start->OnClicked.AddDynamic(this, &UUIStartMenu::StartButtonCallback);
	Exit->OnClicked.AddDynamic(this, &UUIStartMenu::EndButtonCallback);
	//OnButtonLevelUpdated.AddDynamic(this, &UUIStartMenu::RemoveFromParent);
	MySoundCue = LoadObject<USoundWave>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/JYS/Audio/MAMCraft_Start_Audio.MAMCraft_Start_Audio'"));

}
void UUIStartMenu::EndViewport()
{

}
