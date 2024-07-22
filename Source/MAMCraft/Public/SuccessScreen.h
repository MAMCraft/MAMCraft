// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SuccessScreen.generated.h"

/**
 * 
 */
UCLASS()
class MAMCRAFT_API USuccessScreen : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	class UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	class UMediaTexture* MediaTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	class UMediaSource* MediaSource;
public:

	virtual void NativeConstruct();
	UFUNCTION(BlueprintCallable, Category = "Media")
	void PlayVideo();

};
