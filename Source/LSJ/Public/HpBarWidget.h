// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/ProgressBar.h>
#include "HpBarWidget.generated.h"


/**
 * 
 */
UCLASS()
class LSJ_API UHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

	float hpPercent;
public:
	void BindCharacterStat(class UStatComponent* NewCharacterStat);
	void SetHPBar(float percent);
	float GetHPBar();
protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
private:
	TWeakObjectPtr<class UStatComponent> CurrentCharacterStat;
		UPROPERTY()
		class UProgressBar* HPProgressBar;

};
