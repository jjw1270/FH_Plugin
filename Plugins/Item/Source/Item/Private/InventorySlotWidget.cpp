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

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (IsEmpty())
	{
		return;
	}

	ensureMsgf(DragWidgetClass, TEXT("DragWidgetClass is nullptr"));
	UOnDragWidget* DragWidget = Cast<UOnDragWidget>(CreateWidget(GetOwningPlayer(), DragWidgetClass));
	UTexture2D* Image = Cast<UTexture2D>(ItemImage->GetBrush().GetResourceObject());
	DragWidget->SetItemImage(Image);

	UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDragDropOperation::StaticClass()));
	if (!IsValid(DragOperation))
	{
		return;
	}
	DragOperation->DefaultDragVisual = DragWidget;
	DragOperation->SlotInventoryItem = SlotInventoryItem;
	DragOperation->ItemImage = Image;
	DragOperation->Payload = this;

	ClearBindWidget();

	InventoryWidget->GetItemInfoBox()->SetVisibility(ESlateVisibility::Collapsed);
	InventoryWidget->GetItemTrash()->SetVisibility(ESlateVisibility::Visible);

	OutOperation = DragOperation;
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	InventoryWidget->GetItemTrash()->SetVisibility(ESlateVisibility::Collapsed);
	
	UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DragOperation)
	{
		return;
	}

	// If a new item is added to the PrevInventorySlot, Add to New slot
	if (!IsEmpty())
	{
		InventoryWidget->AddNewItemToSlot(DragOperation->SlotInventoryItem);
		return;
	}

	BindOnInventoryItemChanged();
	UpdateItem(DragOperation->SlotInventoryItem);
}

// Called On Drag Droped Slot!
bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	InventoryWidget->GetItemTrash()->SetVisibility(ESlateVisibility::Collapsed);

	UItemDragDropOperation* DragOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DragOperation)
	{
		return false;
	}

	// if slot is empty
	if (!SlotInventoryItem)
	{	
		BindOnInventoryItemChanged();
		UpdateItem(DragOperation->SlotInventoryItem);

		return true;
	}

	// if slot has Item, Swap slots
	UInventorySlotWidget* PrevInventorySlot = Cast<UInventorySlotWidget>(DragOperation->Payload);
	if (PrevInventorySlot)
	{
		PrevInventorySlot->BindOnInventoryItemChanged();
		PrevInventorySlot->UpdateItem(SlotInventoryItem);

		ClearBindWidget();
		BindOnInventoryItemChanged();
		UpdateItem(DragOperation->SlotInventoryItem);

		return true;
	}

	return false;
}

void UInventorySlotWidget::BindOnInventoryItemChanged()
{
	DeleHandle_OnInventoryItemChanged = InventoryComp->Fuc_Dele_OnInventoryItemChanged.AddUObject(this, &UInventorySlotWidget::UpdateItem);
}

void UInventorySlotWidget::UpdateItem(FInventoryItem* NewItem)
{
	if (!SlotInventoryItem)
	{
		SlotInventoryItem = NewItem;
	}

	SetWidgetBindVariables();
}

void UInventorySlotWidget::SetOwningInventoryWidget(UInventoryWidget* NewInventoryWidget)
{
	InventoryWidget = NewInventoryWidget;
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

	if (DeleHandle_OnInventoryItemChanged.IsValid())
	{
		InventoryComp->Fuc_Dele_OnInventoryItemChanged.Remove(DeleHandle_OnInventoryItemChanged);
	}

	Amount = 0;
	ItemType = EItemType::None;
	ItemName = "";
	ItemPrice = 0;
	ItemInfo = "";
	ItemImage->SetBrushFromTexture(nullptr);
	ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));

	return;
}
