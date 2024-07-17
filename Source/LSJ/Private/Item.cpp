// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/WidgetComponent.h"
#include <Kismet/KismetMathLibrary.h>
// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	itemID = 0;
	UseActionText = FText::FromString("Use");

	itemWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	itemWidgetComponent->SetupAttachment(RootComponent);
	itemWidgetComponent->SetRelativeLocation(FVector(0, 0, 0));
	//itemWidgetComponent->SetRelativeRotation();
	itemWidgetComponent->SetWidgetSpace(EWidgetSpace::World);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LSJ/Blueprints/Inventory/WBP_Item.WBP_Item_C'"));
	if (UI_HUD.Succeeded())
	{
		itemWidgetComponent->SetWidgetClass(UI_HUD.Class);
		//itemWidgetComponent->SetDrawSize(FVector2D(1500, 1500));
	}
	itemWidgetComponent->SetCollisionProfileName(TEXT("Item"));
}


// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	itemWidget = Cast<UItemUserWidget>(itemWidgetComponent->GetUserWidgetObject());
	if (nullptr != itemWidget)
	{
		itemWidget->BindItemImage(Thumnail);
		APlayerCameraManager* PlayerCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FVector end = PlayerCamera->GetCameraLocation();

		//FRotator lookRotation = UKismetMathLibrary::FindRelativeLookAtRotation(GetActorTransform(), PlayerCamera->GetCameraLocation());
		FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerCamera->GetCameraLocation());
		SetActorRotation(lookRotation);
		//itemWidgetComponent->SetRelativeRotation(lookRotation);
	
		//FVector newForward = PlayerCamera->GetCameraLocation() - GetActorLocation();
		//newForward.Normalize();

		//const FVector WorldUp(0.0f, 0.0f, 1.0f);

		//FVector newRight = FMath::Cross(PlayerCamera->GetCameraLocation(), WorldUp);
		//FVector newUp = FVector::Cross(newRight, newForward);

		//return FTransform(newForward, newRight, newUp, Source);
	}

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

