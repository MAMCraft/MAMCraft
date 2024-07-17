// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "SuccessScreen.h"
#include "MAMCGameInstance.h"
#include "MAMCGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class JYS_API AMAMCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
    virtual void BeginPlay() override;
		UPROPERTY()
	TSubclassOf<UUserWidget> successHUDWidgetClass;
	USuccessScreen* successHUDWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> inventoryHUDFactory;
	UInventoryWidget* inventoryHUD;
private:
	class UUserWidget* FailUI;
	class UUserWidget* OverUI;

public:
    AMAMCGameModeBase();
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> FailScreen;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> OverScreen;
	
	void showfailscreen();
	void showoverscreen();

	UFUNCTION()
	void EndViewport();
	void VisibleSuccessWidget();
	void HiddenSuccessWidget();
	void VisibleInventoryWidget();
	void HiddenInventoryWidget();
	void Save(TArray<AItem*>& inputItems);
	void Load(TArray<AItem*>& outputItems);
};
