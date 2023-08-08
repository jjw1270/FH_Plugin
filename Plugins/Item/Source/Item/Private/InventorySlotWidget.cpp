// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "ItemInterface.h"
#include "InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "InventoryWidget.h"
#include "ItemDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "OnDragWidget.h"

void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InventoryComp = Cast<UInventoryComponent>(GetOwningPlayer()->GetComponentByClass(UInventoryComponent::StaticClass()));
	ensureMsgf(InventoryComp, TEXT("InventoryComp is nullptr"));

	ClearBindWidget();
}

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ItemInfoBox = InventoryComp->GetInventoryWidget()->GetItemInfoBox();
	ensureMsgf(ItemInfoBox, TEXT("ItemInfoBox is nullptr"));

}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (IsEmpty())
	{
		return;
	}

	ensureMsgf(DragWidgetClass, TEXT("DragWidgetClass is nullptr"));
	UOnDragWidget* DragWidget = Cast<UOnDragWidget>(CreateWidget(GetWorld(), DragWidgetClass));
	DragWidget->SetItemImage(Cast<UTexture2D>(ItemImage->GetBrush().GetResourceObject()));

	UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDragDropOperation::StaticClass()));
	if (!IsValid(DragOperation))
	{
		return;
	}
	DragOperation->DefaultDragVisual = DragWidget;
	DragOperation->SlotInventoryItem = SlotInventoryItem;
	DragOperation->Payload = this;

	ClearBindWidget();

	ItemInfoBox->SetVisibility(ESlateVisibility::Collapsed);
	InventoryComp->GetInventoryWidget()->GetItemTrash()->SetVisibility(ESlateVisibility::Visible);

	OutOperation = DragOperation;
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DragOperation)
	{
		return;
	}

	SetItemDataToSlot(DragOperation->SlotInventoryItem);

	InventoryComp->GetInventoryWidget()->GetItemTrash()->SetVisibility(ESlateVisibility::Collapsed);
}

// Called On Drag Droped Slot!
bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DragOperation)
	{
		return false;
	}

	// if slot is empty
	if (!SlotInventoryItem)
	{	
		SetItemDataToSlot(DragOperation->SlotInventoryItem);

		return true;
	}

	// if slot has Item, Swap slots
	UInventorySlotWidget* PrevInventorySlot = Cast<UInventorySlotWidget>(DragOperation->Payload);
	if (PrevInventorySlot)
	{
		PrevInventorySlot->SetItemDataToSlot(SlotInventoryItem);
		
		SetItemDataToSlot(DragOperation->SlotInventoryItem);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *PrevInventorySlot->GetName());

		return true;
	}

	return false;
}

void UInventorySlotWidget::SetItemDataToSlot(struct FInventoryItem* NewInventoryItem)
{
	SlotInventoryItem = NewInventoryItem;

	SetWidgetBindVariables();
}

void UInventorySlotWidget::UpdateItemAmount()
{
	Amount = SlotInventoryItem->Amount;
}

void UInventorySlotWidget::SetWidgetBindVariables()
{
	ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

	Amount = SlotInventoryItem->Amount;
	ItemType = SlotInventoryItem->Type;

	switch (ItemType)
	{
	case EItemType::Consumable:
	{
		FConsumableItemData* ItemData = InventoryComp->GetConsumableItemInfo(SlotInventoryItem->ID);
		ItemName = ItemData->Name;
		ItemPrice = ItemData->Price;
		ItemInfo = ItemData->ItemTextInfo;
		ItemImage->SetBrushFromTexture(ItemData->ItemImage);
		return;
	}
	case EItemType::Equipment:
	{
		FEquipmentItemData* ItemData = InventoryComp->GetEquipmentItemInfo(SlotInventoryItem->ID);
		ItemName = ItemData->Name;
		ItemPrice = ItemData->Price;
		ItemInfo = ItemData->ItemTextInfo;
		ItemImage->SetBrushFromTexture(ItemData->ItemImage);
		return;
	}
	default:
		return;
	}
}

void UInventorySlotWidget::ClearBindWidget()
{
	SlotInventoryItem = nullptr;

	Amount = 0;
	ItemType = EItemType::None;
	ItemName = "";
	ItemPrice = 0;
	ItemInfo = "";
	ItemImage->SetBrushFromTexture(nullptr);
	ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));

	return;
}
