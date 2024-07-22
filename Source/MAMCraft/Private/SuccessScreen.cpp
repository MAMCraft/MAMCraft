// Fill out your copyright notice in the Description page of Project Settings.


#include "SuccessScreen.h"
//#include "MediaPlayer.h"
//#include "MediaTexture.h"
//#include "MediaSource.h"
#include "Components/Image.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "MediaSource.h"

void USuccessScreen::NativeConstruct()
{
	Super::NativeConstruct();

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
