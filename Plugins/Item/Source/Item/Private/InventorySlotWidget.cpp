// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "ItemInterface.h"
#include "InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "InventoryWidget.h"
#include "ItemDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InventoryComp = Cast<UInventoryComponent>(GetOwningPlayer()->GetComponentByClass(UInventoryComponent::StaticClass()));
	ensureMsgf(InventoryComp, TEXT("InventoryComp is nullptr"));
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (IsEmpty())
	{
		return;
	}
	/////////////////////////////////////////////
	DragWidget = CreateWidget(GetOwningPlayer(), DragWidgetClass);
	DragOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(DragOperationClass));
	DragOperation->DefaultDragVisual = DragWidget;
	DragOperation->SlotInventoryItem = *SlotInventoryItem;
	SetWidgetBindVariables(true);
	ItemInfoBox->SetVisibility(ESlateVisibility::Collapsed);

	OutOperation = DragOperation;
}

void UInventorySlotWidget::SetItemDataToSlot(struct FInventoryItem* NewInventoryItem)
{
	SlotInventoryItem = NewInventoryItem;
	ItemInfoBox = InventoryComp->GetInventoryWidget()->GetItemInfoBox();

	SetWidgetBindVariables(false);
}

void UInventorySlotWidget::UpdateItemAmount()
{
	Amount = SlotInventoryItem->Amount;
}

void UInventorySlotWidget::SetWidgetBindVariables(bool bReset)
{
	if (bReset)
	{
		Amount = 0;
		ItemType = EItemType::None;
		ItemName = "";
		ItemPrice = 0;
		ItemInfo = "";
		ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
		return;
	}

	Amount = SlotInventoryItem->Amount;
	ItemType = SlotInventoryItem->Type;
	ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

	switch (ItemType)
	{
	case EItemType::Consumable:
	{
		FConsumableItemData* ItemData = InventoryComp->GetConsumableItemInfo(SlotInventoryItem->ID);
		ItemName = ItemData->Name;
		ItemPrice = ItemData->Price;
		ItemInfo = ItemData->ItemTextInfo;
		Image = ItemData->ItemImage;
		return;
	}
	case EItemType::Equipment:
	{
		FEquipmentItemData* ItemData = InventoryComp->GetEquipmentItemInfo(SlotInventoryItem->ID);
		ItemName = ItemData->Name;
		ItemPrice = ItemData->Price;
		ItemInfo = ItemData->ItemTextInfo;
		Image = ItemData->ItemImage;
		return;
	}
	default:
		return;
	}
}
