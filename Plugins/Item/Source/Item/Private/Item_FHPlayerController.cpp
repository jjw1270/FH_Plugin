// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHPlayerController.h"
#include "Item_FHGameInstance.h"
//Enhanced Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//UI
#include "InventoryWidget.h"

AItem_FHPlayerController::AItem_FHPlayerController()
{
}

void AItem_FHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(UIMappingContext, 0);
	}

	ensureMsgf(IsValid(InventoryWidgetClass), TEXT("InventoryWidgetClass is Not Valid"));
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
}

void AItem_FHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AItem_FHPlayerController::InventoryUI);
	}
}

void AItem_FHPlayerController::InventoryUI()
{
	ensureMsgf(IsValid(InventoryWidget), TEXT("InventoryWidget is Not Valid"));

	if (!bIsInventoryUIOpen)
	{
		bIsInventoryUIOpen = true;
		//Open Inventory UI
		InventoryWidget->AddToViewport();
		SetShowMouseCursor(true);
		SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		bIsInventoryUIOpen = false;
		//Close Inventory UI
		InventoryWidget->RemoveFromParent();
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
}
