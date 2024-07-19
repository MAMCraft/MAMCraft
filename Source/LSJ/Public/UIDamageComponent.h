// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UIDamageComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LSJ_API UUIDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUIDamageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnyWhere, Category = UI)
	TArray<class UWidgetComponent*> widgetComponentArray;
	UPROPERTY(VisibleAnyWhere, Category = UI)
	TArray<class UUIDamage*> damageBarWidgetArray;
	UPROPERTY(VisibleAnyWhere, Category = UI)
	TArray<class UWidgetComponent*> deleteComponentArray;
	//���̰�
	//���� ��ġ���� ī�޶� �ٶ󺸴� damageRotation, ���󰡾��ϴ� direction, ������ġ homePos
	void SetVisibleDamageUI(FRotator damageRotation, FVector direction, FTransform homePos);
	//�Ⱥ��̰�
	void SetHiidenDamageUI();
	//��Ȱ��ȭ �Ȱ� ����
	void GetDamageUI(UWidgetComponent*& currentComponent, UUIDamage*& currentWidget);
	//��Ȱ��ȭ
	void InActiveDamageUI();
	
	FTimerHandle inActiveDamageUIHandler;
};