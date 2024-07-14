// Fill out your copyright notice in the Description page of Project Settings.


#include "UIStartMenu.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

	// Start 버튼 클릭시 실행될 함수
void UUIStartMenu::StartButtonCallback()
{
	// 레벨을 바꿔주는 함수
	UGameplayStatics::OpenLevel(this, TEXT("Forest"));
	OnButtonLevelUpdated.Broadcast();
	//this->SetVisibility(ESlateVisibility::Hidden);
}

// End 버튼 클릭시 실행될 함수
void UUIStartMenu::EndButtonCallback()
{
	// 게임 종료 함수
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
	RemoveFromParent();
}

void UUIStartMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯 블루프린트의 버튼을 이름을 통해 가져온다
	//Start = Cast<UButton>(GetWidgetFromName(TEXT("Start")));
	//Exit = Cast<UButton>(GetWidgetFromName(TEXT("End")));

	// 버튼 클릭시 호출될 델리게이트에 함수를 등록한다
	Start->OnClicked.AddDynamic(this, &UUIStartMenu::StartButtonCallback);
	Exit->OnClicked.AddDynamic(this, &UUIStartMenu::EndButtonCallback);
	//OnButtonLevelUpdated.AddDynamic(this, &UUIStartMenu::RemoveFromParent);
}
void UUIStartMenu::EndViewport()
{

}
