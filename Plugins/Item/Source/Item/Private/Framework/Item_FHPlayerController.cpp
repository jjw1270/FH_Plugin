// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHPlayerController.h"
// Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
// Components
#include "InventoryComponent.h"
#include "QuickSlotComponent.h"
// UI
#include "Item_FHHUD.h"
#include "Item_HUDWidget.h"

AItem_FHPlayerController::AItem_FHPlayerController()
{
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
	QuickSlotComp = CreateDefaultSubobject<UQuickSlotComponent>(TEXT("QuickSlotComp"));
}

void AItem_FHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(UIMappingContext, 0);
	}
}

void AItem_FHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction<AItem_FHPlayerController, FName>(InventoryUIAction, ETriggerEvent::Started, this, &AItem_FHPlayerController::WidgetOnOff, FName("InventoryWidget"));
		EnhancedInputComponent->BindAction<AItem_FHPlayerController, FName>(EquipmentUIAction, ETriggerEvent::Started, this, &AItem_FHPlayerController::WidgetOnOff, FName("EquipmentWidget"));
	}
}

void AItem_FHPlayerController::WidgetOnOff(FName WidgetName)
{
	if (!HUDWidget)
	{
		AItem_FHHUD* FHHUD = GetHUD<AItem_FHHUD>();
		check(FHHUD);

		HUDWidget = FHHUD->GetHUDWidget();
		check(HUDWidget);
	}

	HUDWidget->SwichWidgetVisibility(this, WidgetName);
}
