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

	    // �޺� ���� ���� �Լ�
//    void StartComboAttack();
//
//    // �޺� ���� ���� �Լ�
//    void EndComboAttack();
//
//private:
//    // Į �޽� ������Ʈ
//    UPROPERTY(VisibleAnywhere, Category = "Components")
//    UStaticMeshComponent* SwordMesh;
//
//    // ���� �̸�
//    FName SocketName;
};
