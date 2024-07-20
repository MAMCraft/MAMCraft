// Fill out your copyright notice in the Description page of Project Settings.


#include "UIDamageComponent.h"
#include "UIDamage.h"
#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
UUIDamageComponent::UUIDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	for (int index = 0; index < 10; index++)
	{
		UWidgetComponent* damageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName(TEXT("DAMAGEUI%d"), index));//
		damageWidgetComponent->SetRelativeLocation(FVector(0, 0, 250.0f));
		damageWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
		damageWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		widgetComponentArray.Add(damageWidgetComponent);
	}
	



	static ConstructorHelpers::FClassFinder<UUserWidget> DamageUIFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/LSJ/Blueprints/Inventory/WBP_Damage2.WBP_Damage2_C'"));
	if (DamageUIFinder.Succeeded())
	{
		for (int index = 0; index < 10; index++)
		{
			widgetComponentArray[index]->SetWidgetClass(DamageUIFinder.Class);
			widgetComponentArray[index]->SetDrawSize(FVector2D(1000, 1000));
		}
	}


}


// Called when the game starts
void UUIDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	for (int index = 0; index < 10; index++)
	{
		widgetComponentArray[index]->InitWidget();
		UUIDamage* damageWidget = Cast<UUIDamage>(widgetComponentArray[index]->GetUserWidgetObject());
		damageWidget->DamageText->SetVisibility(ESlateVisibility::Hidden);
		damageBarWidgetArray.Add(damageWidget);
		if (damageBarWidgetArray.IsValidIndex(index))
		{
			widgetComponentArray[index]->SetActive(false);
		}
	}


}


// Called every frame
void UUIDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	for (int index = 0; index < 10; index++)
	{
		if (widgetComponentArray[index]->IsActive())
		{
			//틱에서 부드럽게 방향에 따라 이동
			widgetComponentArray[index]->AddRelativeLocation(damageBarWidgetArray[index]->Direction() * DeltaTime * 100.0f);
			//틱에서 rotation 유지
			widgetComponentArray[index]->SetRelativeRotation(damageBarWidgetArray[index]->DamageRotation());
		}
	}
}

void UUIDamageComponent::SetVisibleDamageUI(FRotator damageRotation, FVector direction, FTransform homePos,float damageAmount)
{
	UWidgetComponent* currentComponent = nullptr;
	UUIDamage* currentWidget = nullptr;
	//배열에서 비활성된 UI 컴포넌트와 위젯을 가져온다
	GetDamageUI(currentComponent, currentWidget);
	//가져온 컴포넌트를 활성화한다.
	currentComponent->SetActive(true);
	//시작 위치 설정
	FVector location = homePos.GetLocation();
	location += FVector(0, 0, 250);
	currentComponent->SetRelativeLocation(location);
	//Damage 수치를 표시
	currentWidget->SetDamageText(damageAmount);
	//카메라를 바라보게 한다.
	currentWidget->DamageRotation(damageRotation);
	//UI가 이동될 방향을 저장한다.
	currentWidget->Direction(direction);
	deleteComponentArray.Add(currentComponent);
	currentWidget->DamageText->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(inActiveDamageUIHandler, this, &UUIDamageComponent::InActiveDamageUI, 2.0f, false);
	//플레이어와 출동 하지 않게
	//currentComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	//Enemy와 출동 하지 않게
	//currentComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	//Rotator가 유지됨
	currentComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//currentComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	//currentComponent->SetSimulatePhysics(true);
	//currentComponent->SetEnableGravity(false);
	currentWidget->PlayAnimationDamageUI();
}

void UUIDamageComponent::SetHiidenDamageUI()
{

}

void UUIDamageComponent::GetDamageUI(UWidgetComponent*& currentComponent, UUIDamage*& currentWidget)
{
	for (int index = 0; index < 10; index++)
	{
		if (!widgetComponentArray[index]->IsActive())
		{
			currentComponent = widgetComponentArray[index];
			currentWidget = damageBarWidgetArray[index];
			return;
		}
	}
}

void UUIDamageComponent::InActiveDamageUI()
{
	if (deleteComponentArray.IsEmpty())
		return;
	for (auto* component : deleteComponentArray)
	{
		component->Activate(false);
	}
	deleteComponentArray.Empty();
}

