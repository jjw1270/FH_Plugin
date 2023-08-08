// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoveConfirmWidget.h"
#include "Components/Image.h"
#include "ItemDragDropOperation.h"
#include "Item_FHPlayerController.h"
#include "InventoryComponent.h"
#include "InventorySlotWidget.h"

void URemoveConfirmWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(GetOwningPlayer());
	ensureMsgf(PC, TEXT("PC is nullptr"));

	InventoryComp = PC->GetInventoryComp();
	ensureMsgf(InventoryComp, TEXT("InventoryComp is nullptr"));
}

void URemoveConfirmWidget::ShowRemoveConfirm(UItemDragDropOperation* NewItemDragDropOperation)
{
	ItemDragDropOperation = NewItemDragDropOperation;

	ItemImage->SetBrushFromTexture(ItemDragDropOperation->SlotInventoryItem->)
	MaxAmount = ItemDragDropOperation->SlotInventoryItem->Amount;

	SetVisibility(ESlateVisibility::Visible);
}

void URemoveConfirmWidget::RemoveItem(int32 Amount)
{
	if (ItemDragDropOperation)
	{
		InventoryComp->RemoveItemFromInventory(ItemDragDropOperation->SlotInventoryItem, ItemDragDropOperation->SlotInventoryItem->Amount);
	}
}

void URemoveConfirmWidget::OnCancel()
{
	UInventorySlotWidget* PrevInventorySlot = Cast<UInventorySlotWidget>(ItemDragDropOperation->Payload);
	if (PrevInventorySlot)
	{
		PrevInventorySlot->SetItemDataToSlot(ItemDragDropOperation->SlotInventoryItem);
	}

	SetVisibility(ESlateVisibility::Collapsed);
}
