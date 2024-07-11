// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ComboAttack.generated.h"

UCLASS()
class JYS_API AComboAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AComboAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	    // 콤보 어택 시작 함수
//    void StartComboAttack();
//
//    // 콤보 어택 종료 함수
//    void EndComboAttack();
//
//private:
//    // 칼 메시 컴포넌트
//    UPROPERTY(VisibleAnywhere, Category = "Components")
//    UStaticMeshComponent* SwordMesh;
//
//    // 소켓 이름
//    FName SocketName;
};
