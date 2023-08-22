// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotSlotWidget.h"
#include "Item.h"
#include "ItemData.h"
#include "InventoryComponent.h"
#include "QuickSlotComponent.h"
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
	CHECK_VALID(InventoryComp);

	QuickSlotComp = InventoryComp->GetQuickSlotComp();
	CHECK_VALID(QuickSlotComp);

	ClearSlot();
}

// Only for inventory slot to quick slot
bool UQuickSlotSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{	
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DDOperation)
	{
		return false;
	}

	// Quick slot is only for Consumalbe Items
	FBaseItemData DraggedBaseIteData;

	if (!DDOperation->DraggingItemData->GetBaseData(DraggedBaseIteData) || DraggedBaseIteData.ItemType != EItemType::Consumable)
	{
		return false;
	}

	QuickSlotComp->SetItemToQuickSlot(Index, DDOperation->DraggingItemData, DDOperation->DraggingItemAmount); 

	return false;
}

FReply UQuickSlotSlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	if (IsEmpty())
	{
		return FReply::Unhandled();
	}

	//check left mouse double clicked
	TSet<FKey> PressedBtns = InMouseEvent.GetPressedButtons();
	for (const auto& pb : PressedBtns)
	{
		if (pb != EKeys::LeftMouseButton)
		{
			return FReply::Unhandled();
		}
	}

	QuickSlotComp->DeleteItemFromQuickSlot(Index, SlotItemData);

	return FReply::Handled();
}

void UQuickSlotSlotWidget::SetSlot(class UItemData* NewItemData, const int32& NewItemAmount)
{
	if (NewItemAmount <= 0)
	{
		ClearSlot();
		return;
	}

	if (!NewItemData)
	{
		ItemImageWidget->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}

	SlotItemData = NewItemData;
	SlotItemAmount = NewItemAmount;

	FBaseItemData BaseItemData;
	if (SlotItemData->GetBaseData(BaseItemData))
	{
		ItemImageWidget->SetBrushFromTexture(BaseItemData.Icon2D);
	}

	// bind ItemUpdateDelegate
	if (!ItemUpdateDelegateHandle.IsValid())
	{
		ItemUpdateDelegateHandle = InventoryComp->ItemUpdateDelegate.AddUObject(this, &UQuickSlotSlotWidget::OnUpdateItem);
	}
}

void UQuickSlotSlotWidget::OnUpdateItem(class UItemData* UpdateItemData, const int32& UpdateAmount)
{
	SlotItemData = UpdateItemData;
	SlotItemAmount = UpdateAmount;
}

void UQuickSlotSlotWidget::ClearSlot()
{
	SlotItemData = nullptr;
	SlotItemAmount = 0;

	ItemImageWidget->SetBrushFromTexture(nullptr);
	ItemImageWidget->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));

	if (ItemUpdateDelegateHandle.IsValid())
	{
		InventoryComp->ItemUpdateDelegate.Remove(ItemUpdateDelegateHandle);
	}
}
