// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHPlayerController.h"
//Components
#include "InventoryComponent.h"
//Enhanced Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
	if (!bIsInventoryUIOpen)
	{
		bIsInventoryUIOpen = true;
		//Open Inventory UI
		InventoryComp->GetInventoryWidget()->AddToViewport();
		SetShowMouseCursor(true);
		SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		bIsInventoryUIOpen = false;
		//Close Inventory UI
		InventoryComp->GetInventoryWidget()->RemoveFromParent();
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
}
