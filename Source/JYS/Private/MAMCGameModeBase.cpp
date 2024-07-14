// Fill out your copyright notice in the Description page of Project Settings.


#include "MAMCGameModeBase.h"



void AMAMCGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    if (IsValid(MainHUDWidgetClass))
    {
        MainHUDWidget = Cast<UUIStartMenu>(CreateWidget(GetWorld(), MainHUDWidgetClass));

        if (IsValid(MainHUDWidget))
        {
            MainHUDWidget->AddToViewport();
           // MainHUDWidget->OnButtonLevelUpdated.AddDynamic(this,&AMAMCGameModeBase::EndViewport);
        }
    }
}

AMAMCGameModeBase::AMAMCGameModeBase()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> MainHUDWidgetAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KYH/Blueprint/BP_StartMenu.BP_StartMenu_C'"));

    // TSubclassOf ���ø� Ŭ���� ��ü�� �������Ʈ Ŭ������ �־��ش�
    if (MainHUDWidgetAsset.Succeeded())
        MainHUDWidgetClass = MainHUDWidgetAsset.Class;

}

void AMAMCGameModeBase::EndViewport()
{
    UE_LOG(LogTemp, Log, TEXT("EndViewport"));
    //MainHUDWidget->FinishDestroy();
}

