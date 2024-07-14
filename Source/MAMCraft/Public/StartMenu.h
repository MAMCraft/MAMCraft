// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartMenu.generated.h"

/**
 * 
 */
UCLASS()
class MAMCRAFT_API UStartMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* StartButton;

	virtual void NativeConstruct() override; // NativeConstruct �Լ� ���� �߰�
private:
	UFUNCTION(BlueprintCallable)
	void StartButtonCallback();
};