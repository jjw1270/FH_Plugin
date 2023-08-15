// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotSlotWidget.h"
#include "ItemType.h"
#include "InventoryComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ItemDragDropOperation.h"
#include "OnDragWidget.h"
#include "Components/Image.h"
#include "InventoryWidget.h"

void UQuickSlotSlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UUserWidget::NativeOnDragCancelled(InDragDropEvent, InOperation);

	InventoryWidget->GetItemTrash()->SetVisibility(ESlateVisibility::Collapsed);

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DDOperation)
	{
		return;
	}

	//SetSlot(DDOperation->DragingItemID);
}

bool UQuickSlotSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UUserWidget::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DDOperation)
	{
		return false;
	}

	UInventorySlotWidget* PrevInventorySlot = Cast<UInventorySlotWidget>(DDOperation->Payload);
	if (!PrevInventorySlot)
	{
		return false;
	}
	InventoryWidget = PrevInventorySlot->GetOwningInventoryWidget();

	InventoryWidget->GetItemTrash()->SetVisibility(ESlateVisibility::Collapsed);

	// Quick slot is only for Consumalbe Items
	if (InventoryComp->GetItemType(DDOperation->DragingItemID) != EItemType::Consumable)
	{
		return false;
	}

	// if quick slot to quick slot
	if (Cast<UQuickSlotSlotWidget>(PrevInventorySlot))
	{
		// if dropped quick slot is empty
		if (ItemID == 0)
		{
			SetSlot(DDOperation->DragingItemID);

			return true;
		}

		// else swap slots
		PrevInventorySlot->SetSlot(ItemID);

		ClearSlot();
		SetSlot(DDOperation->DragingItemID);

		return true;
	}
	// if inventory slot to quick slot, duplicate item
	else
	{
		// if dropped quick slot is empty
		if (ItemID == 0)
		{
			SetSlot(DDOperation->DragingItemID);

			return false;
		}

		// else clear dropped quick slot first
		ClearSlot();
		SetSlot(DDOperation->DragingItemID);

		return false;
	}
}

void UQuickSlotSlotWidget::SetWidgetBindVariables()
{
	Amount = *InventoryComp->GetInventoryItems()->Find(ItemID);

	FConsumableItemData* ItemData = InventoryComp->GetConsumableItemInfo(ItemID);
	ItemImageWidget->SetBrushFromTexture(ItemData->ItemImage);
}

void UQuickSlotSlotWidget::ClearSlot()
{
	ItemID = 0;

	if (!OnInventoryItemChangedHandle.IsValid())
	{
		InventoryComp->OnInventoryItemChangedDelegate.Remove(OnInventoryItemChangedHandle);
	}

	Amount = 0;
	ItemImageWidget->SetBrushFromTexture(nullptr);
	ItemImageWidget->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
}
