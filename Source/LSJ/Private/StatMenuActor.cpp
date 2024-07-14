// Fill out your copyright notice in the Description page of Project Settings.


#include "StatMenuActor.h"

// Sets default values
AStatMenuActor::AStatMenuActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<UUserWidget> MainHUDWidgetAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KYH/Blueprint/BP_StartMenu.BP_StartMenu_C'"));

	// TSubclassOf 템플릿 클래스 객체에 블루프린트 클래스를 넣어준다
	if (MainHUDWidgetAsset.Succeeded())
		MainHUDWidgetClass = MainHUDWidgetAsset.Class;
}

// Called when the game starts or when spawned
void AStatMenuActor::BeginPlay()
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

// Called every frame
void AStatMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

