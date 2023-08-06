// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "InventorySlotWidget.h"

#include "ItemInterface.h"
#include "InventoryComponent.h"
#include "Item_FHPlayerController.h"

void UInventoryWidget::NativeOnInitialized()
{
	AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(GetOwningPlayer());
	ensureMsgf(PC, TEXT("PC is nullptr"));

	InventoryComp = PC->GetInventoryComp();
	ensureMsgf(InventoryComp, TEXT("InventoryComp is nullptr"));
}

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (bIsCreateSlots) return;
	bIsCreateSlots = true;

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

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWidget::NativeDestruct()
{

}

void UInventoryWidget::SetItemsToSlots()
{
	TArray<FInventoryItem>* InventoryItemArray = InventoryComp->GetInventoryItems();

	for (auto& InventoryItem : *InventoryItemArray)
	{
		for (auto slot : InventorySlotArray)
		{
			if (slot->IsEmpty())
			{
				slot->SetItemDataToSlot(InventoryItem);
				UE_LOG(LogTemp, Warning, TEXT("aaa"));
			}
		}
	}
}
