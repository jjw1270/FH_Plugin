// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "Item.h"
#include "ItemData.h"
// Drag Drop Operation
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ItemDragDropOperation.h"
#include "OnDragWidget.h"
// Components
#include "InventoryComponent.h"
#include "QuickSlotComponent.h"


#include "InventoryWidget.h"

#include "Components/Image.h"

#include "Components/TextBlock.h"
// #include "Item_FHPlayerController.h"
#include "Item_HUDWidget.h"
#include "QuickSlotWidget.h"
#include "QuickSlotSlotWidget.h"
#include "EquipmentComponent.h"
#include "Item_FHPlayerState.h"

void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InventoryComp = Cast<UInventoryComponent>(GetOwningPlayer()->GetComponentByClass(UInventoryComponent::StaticClass()));
	ensureMsgf(InventoryComp, TEXT("InventoryComp is nullptr"));

	ClearSlot();
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return reply.NativeReply;
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (IsEmpty())
	{
		return;
	}

	CHECK_VALID(DragWidgetClass);
	UOnDragWidget* DragWidget = Cast<UOnDragWidget>(CreateWidget(GetOwningPlayer(), DragWidgetClass));

	FBaseItemData BaseItemData;
	if (!SlotItemData->GetBaseData(BaseItemData))
	{
		return;
	}

	DragWidget->SetItemImage(BaseItemData.Icon2D);

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDragDropOperation::StaticClass()));
	CHECK_VALID(DDOperation);

	DDOperation->DefaultDragVisual = DragWidget;
	DDOperation->DraggingItemData = SlotItemData;
	DDOperation->DraggingItemAmount = SlotItemAmount;
	
	DDOperation->Payload = this;

	// Not Delete Item from Inventory, just Clear in Widget
	ClearSlot();

	InventoryWidget->GetItemInfoBox()->SetVisibility(ESlateVisibility::Collapsed);
	InventoryWidget->GetItemTrash()->SetVisibility(ESlateVisibility::Visible);

	OutOperation = DDOperation;
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	InventoryWidget->GetItemTrash()->SetVisibility(ESlateVisibility::Collapsed);

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(InOperation);
	CHECK_VALID(DDOperation);

	// If a new item is added to the PrevInventorySlot, Add to New slot
	if (!IsEmpty())
	{
		InventoryWidget->AddNewItemToSlot(DDOperation->DraggingItemData, DDOperation->DraggingItemAmount);
		return;
	}

	SetSlot(DDOperation->DraggingItemData, DDOperation->DraggingItemAmount);
}

// Called On Drag Droped Slot!
bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	InventoryWidget->GetItemTrash()->SetVisibility(ESlateVisibility::Collapsed);

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DDOperation)
	{
		return false;
	}

	// if slot is empty
	if (IsEmpty())
	{	
		SetSlot(DDOperation->DraggingItemData, DDOperation->DraggingItemAmount);
		return true;
	}

	// else slot has Item, Swap slots
	UInventorySlotWidget* PrevInventorySlot = Cast<UInventorySlotWidget>(DDOperation->Payload);
	if (!PrevInventorySlot)
	{
		return false;
	}

	PrevInventorySlot->SetSlot(SlotItemData, SlotItemAmount);
	
	ClearSlot();
	SetSlot(DDOperation->DraggingItemData, DDOperation->DraggingItemAmount);

	return true;
}

FReply UInventorySlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

	FBaseItemData BaseItemData;
	if (!SlotItemData->GetBaseData(BaseItemData))
	{
		return FReply::Unhandled();
	}

	// Consumable Items
	if (BaseItemData.ItemType == EItemType::Consumable)
	{
		UQuickSlotComponent* QuickSlotComponent = InventoryComp->GetQuickSlotComp();
		if (!QuickSlotComponent)
		{
			return FReply::Unhandled();
		}

		// Check QuickSlot is empty
		int32 QuickSlotIndex = QuickSlotComponent->GetEmptyQuickSlotSlotIndex();
		if (QuickSlotIndex < 0)
		{
			return FReply::Unhandled();
		}

		QuickSlotComponent->SetItemToQuickSlot(QuickSlotIndex, SlotItemData, SlotItemAmount);

		return FReply::Handled();
	}

	// Weapon, Armor Items
	if (BaseItemData.ItemType == EItemType::Weapon || BaseItemData.ItemType == EItemType::Armor)
	{
		///////////////////////////////////////////////////////////////////////////
	}
}

void UInventorySlotWidget::SetOwningInventoryWidget(UInventoryWidget* NewInventoryWidget)
{
	InventoryWidget = NewInventoryWidget;
}

void UInventorySlotWidget::SetSlot(class UItemData* NewItemData, const int32& NewAmount)
{
	if (NewAmount <= 0)
	{
		ClearSlot();
		return;
	}

	if (!SlotItemData)
	{
		ItemImageWidget->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}

	SlotItemData = NewItemData;
	SlotItemAmount = NewAmount;

	FBaseItemData BaseItemData;
	if (SlotItemData->GetBaseData(BaseItemData))
	{
		ItemImageWidget->SetBrushFromTexture(BaseItemData.Icon2D);
	}

	// If InfoBox is on Visible, Set Collapsed
	if (InventoryWidget)
	{
		UUserWidget* ItemInfoBox = InventoryWidget->GetItemInfoBox();
		if (ItemInfoBox->GetVisibility() == ESlateVisibility::Visible)
		{
			ItemInfoBox->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UInventorySlotWidget::ClearSlot()
{
	SlotItemData = nullptr;
	SlotItemAmount = 0;

	ItemImageWidget->SetBrushFromTexture(nullptr);
	ItemImageWidget->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
}
