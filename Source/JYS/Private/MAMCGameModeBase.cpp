// Fill out your copyright notice in the Description page of Project Settings.


#include "MAMCGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "FailScreen.h"
#include "Kismet/GameplayStatics.h"
#include "OverScreen.h"



void AMAMCGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    if (IsValid(successHUDWidgetClass))
    {
        successHUDWidget = Cast<USuccessScreen>(CreateWidget(GetWorld(), successHUDWidgetClass));

        if (IsValid(successHUDWidget))
        {
            successHUDWidget->AddToViewport();
            successHUDWidget->SetVisibility(ESlateVisibility::Hidden);
            // MainHUDWidget->OnButtonLevelUpdated.AddDynamic(this,&AMAMCGameModeBase::EndViewport);
        }
    }

    //LSJ 인벤토리
    //inventoryHUD->initwidget
    if (!IsValid(inventoryHUD))
    {
        inventoryHUD = Cast<UInventoryWidget>(CreateWidget(GetWorld(), inventoryHUDFactory));
        if (inventoryHUD)
        {
            //inventoryHUD->SetVisibility(ESlateVisibility::Hidden);
            //inventoryHUD->AddToViewport();
        }
    }

}

AMAMCGameModeBase::AMAMCGameModeBase()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> successHUDWidgetAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KYH/Blueprint/BP_SuceessScreen.BP_SuceessScreen_C'"));

    // TSubclassOf 템플릿 클래스 객체에 블루프린트 클래스를 넣어준다
    if (successHUDWidgetAsset.Succeeded())
        successHUDWidgetClass = successHUDWidgetAsset.Class;

    static ConstructorHelpers::FClassFinder<UUserWidget> inventoryHUDClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LSJ/Blueprints/Inventory/BP_InventoryWidget1.BP_InventoryWidget1_C'"));
    if (inventoryHUDClass.Succeeded())
    {
        inventoryHUDFactory = inventoryHUDClass.Class;
    }
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


void AMAMCGameModeBase::EndViewport()
{
    //UE_LOG(LogTemp, Log, TEXT("EndViewport"));
    //MainHUDWidget->FinishDestroy();
}

void AMAMCGameModeBase::VisibleSuccessWidget()
{
    successHUDWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMAMCGameModeBase::HiddenSuccessWidget()
{
    successHUDWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMAMCGameModeBase::VisibleInventoryWidget()
{
    inventoryHUD->AddToViewport();
    //inventoryHUD->SetVisibility(ESlateVisibility::Visible);
}

void AMAMCGameModeBase::HiddenInventoryWidget()
{
    inventoryHUD->SetVisibility(ESlateVisibility::Hidden);
}

void AMAMCGameModeBase::Save(TArray<AItem*>& inputItems)
{
    UMAMCGameInstance* instanceMAMC = Cast<UMAMCGameInstance>(GetGameInstance());
    instanceMAMC->Save(inputItems);
}

void AMAMCGameModeBase::Load(TArray<AItem*>& outputItems)
{
    UMAMCGameInstance* instanceMAMC = Cast<UMAMCGameInstance>(GetGameInstance());
    instanceMAMC->Load(outputItems);
}



