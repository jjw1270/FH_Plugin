// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotSlotWidget.h"
#include "ItemType.h"
#include "InventoryComponent.h"
#include "Item_FHPlayerController.h"
#include "Item_HUDWidget.h"
#include "QuickSlotWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ItemDragDropOperation.h"
#include "Components/Image.h"

void UQuickSlotSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InventoryComp = Cast<UInventoryComponent>(GetOwningPlayer()->GetComponentByClass(UInventoryComponent::StaticClass()));
	ensureMsgf(InventoryComp, TEXT("InventoryComp is nullptr"));

	ClearSlot();
}

FReply UQuickSlotSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetPressedButtons().Contains(EKeys::RightMouseButton))
	{
		if (!IsEmpty())
		{
			ClearSlot();
		}
	}

	return FReply::Handled();
}

// Only for inventory slot to quick slot
bool UQuickSlotSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{	
	UUserWidget::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DDOperation)
	{
		return false;
	}

	// Quick slot is only for Consumalbe Items
	if (InventoryComp->GetItemType(DDOperation->DragingItemID) != EItemType::Consumable)
	{
		return false;
	}

	// check quickslotslots in quickslotslotarray has same Item : clear prev quick slot item
	if (!QuickSlotWidget)
	{
		QuickSlotWidget = GetOwningPlayer<AItem_FHPlayerController>()->GetHUDWidget()->GetQuickSlotWidget();
	}

	for (auto QuickSlotSlot : *QuickSlotWidget->GetQuickSlotSlotArray())
	{
		if (QuickSlotSlot->GetSlotItemID() == DDOperation->DragingItemID)
		{
			QuickSlotSlot->ClearSlot();
		}
	}

	//duplicate inventory slot to quick slot
	SetSlot(DDOperation->DragingItemID);

	return false;
}

void UQuickSlotSlotWidget::SetSlot(const int32& NewItemID)
{
	if (!OnInventoryItemChangedHandle.IsValid())
	{
		OnInventoryItemChangedHandle = InventoryComp->OnInventoryItemChangedDelegate.AddUObject(this, &UQuickSlotSlotWidget::OnUpdateItem);
	}

	ItemID = NewItemID;

	ItemImageWidget->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

	SetWidgetBindVariables();
}

void UQuickSlotSlotWidget::OnUpdateItem(const int32& UpdateItemID)
{
	if (ItemID == UpdateItemID)
	{
		// if This Item Fully Removed
		if (!InventoryComp->GetInventoryItems()->Contains(ItemID))
		{
			ClearSlot();
			return;
		}

		SetWidgetBindVariables();
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
