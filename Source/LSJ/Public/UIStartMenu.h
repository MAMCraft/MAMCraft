// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIStartMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonLevelUpdated);
/**
 * 
 */
UCLASS()
class LSJ_API UUIStartMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	// ��ư�� ������ ��, ȣ��� ��������Ʈ�� ����� �Լ�
	UFUNCTION(BlueprintCallable)
	void StartButtonCallback();

	UFUNCTION(BlueprintCallable)
	void EndButtonCallback();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Start;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Exit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* VideoPlayer;

	virtual void NativeConstruct();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	class UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	class UMediaTexture* MediaTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	class UMediaSource* MediaSource;

public:
	UFUNCTION(BlueprintCallable, Category = "Media")
	void PlayVideo();
	UPROPERTY(BlueprintAssignable)
	FOnButtonLevelUpdated OnButtonLevelUpdated;
		UFUNCTION()
	void EndViewport();
	
	FTimerHandle TimerHandle;

	UFUNCTION()
	void TestFunction();

};
