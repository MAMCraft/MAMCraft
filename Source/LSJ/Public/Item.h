// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemUserWidget.h"
#include "Item.generated.h"

UENUM()
enum class EItemCategroy : int
{
	posion,
	sword,
	bow,
	arrow
};

UCLASS()
class LSJ_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Transient)
	class UWorld* World;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ITEM")
	FText UseActionText;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ITEM")
	class UStaticMesh* PickupMesh;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ITEM")
	class UTexture2D* Thumnail;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ITEM")
	FText ItemDisplayName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ITEM",meta=(MultiLine = true))
	FText ItemDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ITEM",meta = (ClampMin = 0.0))
	float Weght;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ITEM",meta = (ClampMin = 0.0))
	int count;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ITEM",meta = (ClampMin = 0.0))
	int category;
	UPROPERTY()
	class UInventoryComponent* OwningInventory;
	//아이템 사용
	virtual void Use() PURE_VIRTUAL(AItem,);
	//widget
	UPROPERTY(VisibleAnyWhere, Category = UI)
	class UWidgetComponent* itemWidgetComponent;
	UItemUserWidget* itemWidget;
};
