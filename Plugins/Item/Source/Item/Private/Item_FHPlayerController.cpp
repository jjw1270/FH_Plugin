// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHPlayerController.h"
// Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
// Components
#include "InventoryComponent.h"
// UI
#include "Item_HUDWidget.h"
#include "InventoryWidget.h"

AItem_FHPlayerController::AItem_FHPlayerController()
{
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
}

void AItem_FHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(UIMappingContext, 0);
	}

	// Create HUDWidget
	ensureMsgf(HUDWidgetClass, TEXT("HudWidgetClass is not set."));
	HUDWidget = CreateWidget<UItem_HUDWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->AddToViewport();
}

void AItem_FHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, InventoryComp, &UInventoryComponent::InventoryUI);
	}
}
