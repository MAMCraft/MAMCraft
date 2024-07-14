// Fill out your copyright notice in the Description page of Project Settings.


#include "UIStartMenu.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

	// Start ��ư Ŭ���� ����� �Լ�
void UUIStartMenu::StartButtonCallback()
{
	// ������ �ٲ��ִ� �Լ�
	UGameplayStatics::OpenLevel(this, TEXT("Forest"));
	OnButtonLevelUpdated.Broadcast();
	//this->SetVisibility(ESlateVisibility::Hidden);
}

// End ��ư Ŭ���� ����� �Լ�
void UUIStartMenu::EndButtonCallback()
{
	// ���� ���� �Լ�
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
	RemoveFromParent();
}

void UUIStartMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� �������Ʈ�� ��ư�� �̸��� ���� �����´�
	//Start = Cast<UButton>(GetWidgetFromName(TEXT("Start")));
	//Exit = Cast<UButton>(GetWidgetFromName(TEXT("End")));

	// ��ư Ŭ���� ȣ��� ��������Ʈ�� �Լ��� ����Ѵ�
	Start->OnClicked.AddDynamic(this, &UUIStartMenu::StartButtonCallback);
	Exit->OnClicked.AddDynamic(this, &UUIStartMenu::EndButtonCallback);
	//OnButtonLevelUpdated.AddDynamic(this, &UUIStartMenu::RemoveFromParent);
}
void UUIStartMenu::EndViewport()
{

}
