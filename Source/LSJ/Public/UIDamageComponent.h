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
	//보이게
	//맞은 위치에서 카메라를 바라보는 damageRotation, 날라가야하는 direction, 시작위치 homePos
	void SetVisibleDamageUI(FRotator damageRotation, FVector direction, FTransform homePos);
	//안보이게
	void SetHiidenDamageUI();
	//비활성화 된거 리턴
	void GetDamageUI(UWidgetComponent*& currentComponent, UUIDamage*& currentWidget);
	//비활성화
	void InActiveDamageUI();
	
	FTimerHandle inActiveDamageUIHandler;
};