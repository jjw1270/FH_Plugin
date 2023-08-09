// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "InventorySlotWidget.h"
#include "InventoryComponent.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PC = Cast<AItem_FHPlayerController>(GetOwningPlayer());
	ensureMsgf(PC, TEXT("PC is nullptr"));

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

void UInventoryWidget::AddItemToSlot(FInventoryItem* NewItem)
{
	for (auto slot : InventorySlotArray)
	{
		if (slot->IsEmpty())
		{
			slot->SetItemDataToSlot(NewItem);
			return;
		}
	}
}

void UInventoryWidget::UpdateItemToSlot(FInventoryItem* NewItem)
{
	for (auto slot : InventorySlotArray)
	{
		if (FInventoryItem* SlotInventoryItem =  slot->GetSlotInventoryItem())
		{
			if (SlotInventoryItem->ID == NewItem->ID)
			{
				slot->UpdateItemAmount();
				return;
			}
		}
	}
}

// Sort according to IsEmpty()
void UInventoryWidget::SortItemSlot()
{
	for (int i = 0; i < InventorySlotArray.Num(); i++)
	{
		if (!InventorySlotArray[i]->IsEmpty())
		{
			continue;
		}

		for (int j = i; j < InventorySlotArray.Num(); j++)
		{
			if (!InventorySlotArray[j]->IsEmpty())
			{
				InventorySlotArray[i]->SetItemDataToSlot(InventorySlotArray[j]->GetSlotInventoryItem());
				InventorySlotArray[j]->ClearBindWidget();

				break;
			}
		}
	}
}
