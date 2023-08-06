// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "InventorySlotWidget.h"
#include "Item_FHGameInstance.h"
#include "ItemInterface.h"
#include "Item_FHPlayerController.h"

void UInventoryWidget::NativeOnInitialized()
{
	GameInstance = Cast<UItem_FHGameInstance>(GetGameInstance());
	ensureMsgf(GameInstance, TEXT("GameInstance is nullptr"));

	ensureMsgf(InventorySlotClass, TEXT("InventorySlotClass is nullptr"));

	for (int32 row = 0; row < SlotGridRowRange; row++)
	{
		for (int32 col = 0; col < SlotGridColRange; col++)
		{
			UInventorySlotWidget* NewInventorySlot = Cast<UInventorySlotWidget>(CreateWidget(GetWorld(), InventorySlotClass));
			InventorySlotGrid->AddChildToUniformGrid(NewInventorySlot, row, col);
			InventorySlotArray.Add(NewInventorySlot);
		}
	}
}

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AItem_FHPlayerController>(GameInstance->GetFirstLocalPlayerController(GetWorld()));
	ensureMsgf(PlayerController, TEXT("PlayerController is nullptr"));

	PlayerController->GetWorldTimerManager().SetTimer(UpdateInventoryHandle, this, &UInventoryWidget::SetItemsToSlots, 0.01f, true);
}

void UInventoryWidget::NativeDestruct()
{
	if (UpdateInventoryHandle.IsValid())
	{
		PlayerController->GetWorldTimerManager().ClearTimer(UpdateInventoryHandle);
	}
}

void UInventoryWidget::SetItemsToSlots()
{
	TArray<FInventoryItem>& InventoryItemArray = GameInstance->GetInventoryItemArray();

	for (int32 i = 0; i < InventoryItemArray.Num(); i++)
	{
		InventorySlotArray[i]->SetItemDataToSlot(InventoryItemArray[i]);
	}
}
