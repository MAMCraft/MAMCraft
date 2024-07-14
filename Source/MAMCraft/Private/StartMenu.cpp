// Fill out your copyright notice in the Description page of Project Settings.

#include "StartMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UStartMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// ���� �������Ʈ�� ��ư�� �̸��� ���� �����´�
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("Start")));

	// ��ư Ŭ���� ȣ��� ��������Ʈ�� �Լ��� ����Ѵ�
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UStartMenu::StartButtonCallback);
	}
}

void UStartMenu::StartButtonCallback()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Forest"));
}
