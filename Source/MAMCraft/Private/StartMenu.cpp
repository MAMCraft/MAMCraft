// Fill out your copyright notice in the Description page of Project Settings.

#include "StartMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UStartMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯 블루프린트의 버튼을 이름을 통해 가져온다
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("Start")));

	// 버튼 클릭시 호출될 델리게이트에 함수를 등록한다
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UStartMenu::StartButtonCallback);
	}
}

void UStartMenu::StartButtonCallback()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Forest"));
}
