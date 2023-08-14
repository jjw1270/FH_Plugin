// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "InventorySlotWidget.h"
#include "InventoryComponent.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CreateSlotWidgets(DefaultSlotGridRowRange);
}

void UInventoryWidget::CreateSlotWidgets(int32 Row)
{
	ensureMsgf(InventorySlotClass, TEXT("InventorySlotClass is nullptr"));
	for (int32 row = 0; row < Row; row++)
	{
		for (int32 col = 0; col < SlotGridColRange; col++)
		{
			UInventorySlotWidget* NewInventorySlot = Cast<UInventorySlotWidget>(CreateWidget(GetOwningPlayer(), InventorySlotClass));
			NewInventorySlot->SetOwningInventoryWidget(this);
			InventorySlotGrid->AddChildToUniformGrid(NewInventorySlot, row, col);
			InventorySlotArray.Add(NewInventorySlot);
		}
	}
}

void UInventoryWidget::AddNewItemToSlot(const int32& ItemID)
{
	for (auto slot : InventorySlotArray)
	{
		if (slot->IsEmpty())
		{
			slot->SetSlot(ItemID);
			return;
		}
	}

	//If All Slots are Full, Make New Slots Row
	CreateSlotWidgets(1);
	AddNewItemToSlot(ItemID);
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

		for (int j = InventorySlotArray.Num()-1; j > i; j--)
		{
			if (!InventorySlotArray[j]->IsEmpty())
			{
				InventorySlotArray[i]->SetSlot(InventorySlotArray[j]->GetSlotItemID());
				InventorySlotArray[j]->ClearSlot();

				break;
			}
		}
	}
}
