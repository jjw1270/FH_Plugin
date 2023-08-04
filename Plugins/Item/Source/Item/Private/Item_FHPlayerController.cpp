// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHPlayerController.h"
//Actor Component
#include "InventoryComponent.h"
//Enhanced Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
		UE_LOG(LogTemp, Warning, TEXT("A"));
	}
	else
	{
		bIsInventoryUIOpen = false;
		//Close Inventory UI
		UE_LOG(LogTemp, Warning, TEXT("B"));

	}
}
