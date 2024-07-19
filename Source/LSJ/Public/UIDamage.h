// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "UIDamage.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UUIDamage : public UUserWidget
{
	GENERATED_BODY()
	
	FRotator damageRotation;
	FVector direction;
public:
	virtual void SetDamageText(int amount);
	virtual int GetDamageText();
	virtual void PlayAnimationDamageUI();
	virtual void HiddenText();
	virtual void VisibleText();
	//사용하는 곳에서 보이게 하고
	//오브젝트 풀 사용

	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	UTextBlock* DamageText;
	//애니메이션 사라지는거 추가
	UPROPERTY(BlueprintReadOnly, Transient, Category = "GsUI",meta=(BindWidgetAnim))
	UWidgetAnimation* DamageOpacity;

	FWidgetAnimationDynamicEvent damageWidgetAnimationEvent;
	
	FRotator DamageRotation() const { return damageRotation; }
	void DamageRotation(FRotator val) { damageRotation = val; }
	FVector Direction() const { return direction; }
	void Direction(FVector val) { direction = val; }

	UFUNCTION()
	void OnEndPlayAnimation();
};
