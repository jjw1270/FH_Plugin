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
#include "ItemType.h"

void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InventoryComp = Cast<UInventoryComponent>(GetOwningPlayer()->GetComponentByClass(UInventoryComponent::StaticClass()));
	ensureMsgf(InventoryComp, TEXT("InventoryComp is nullptr"));

	ClearSlot();
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
	UTexture2D* Image = GetItemImage();
	DragWidget->SetItemImage(Image);

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDragDropOperation::StaticClass()));
	if (!IsValid(DDOperation))
	{
		return;
	}

	DDOperation->DefaultDragVisual = DragWidget;
	DDOperation->DragingItemID = ItemID;
	DDOperation->Payload = this;

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
	if (!DDOperation)
	{
		return;
	}

	// If a new item is added to the PrevInventorySlot, Add to New slot
	if (!IsEmpty())
	{
		InventoryWidget->AddNewItemToSlot(DDOperation->DragingItemID);
		return;
	}

	SetSlot(DDOperation->DragingItemID);
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
	if (ItemID == 0)
	{	
		SetSlot(DDOperation->DragingItemID);

		return true;
	}

	// else slot has Item, Swap slots
	UInventorySlotWidget* PrevInventorySlot = Cast<UInventorySlotWidget>(DDOperation->Payload);
	if (!PrevInventorySlot)
	{
		return false;
	}

	PrevInventorySlot->SetSlot(ItemID);
	
	SetSlot(DDOperation->DragingItemID);

	return true;
}

void UInventorySlotWidget::SetOwningInventoryWidget(UInventoryWidget* NewInventoryWidget)
{
	InventoryWidget = NewInventoryWidget;
}

void UInventorySlotWidget::SetSlot(const int32& NewItemID)
{
	if (!OnInventoryItemChangedHandle.IsValid())
	{
		OnInventoryItemChangedHandle = InventoryComp->OnInventoryItemChangedDelegate.AddUObject(this, &UInventorySlotWidget::OnUpdateItem);
	}

	ItemID = NewItemID;

	ItemImageWidget->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

	SetWidgetBindVariables();
}

void UInventorySlotWidget::OnUpdateItem(const int32& UpdateItemID)
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

void UInventorySlotWidget::SetWidgetBindVariables()
{
	ItemType = InventoryComp->GetItemType(ItemID);
	
	Amount = *InventoryComp->GetInventoryItems()->Find(ItemID);

	switch (ItemType)
	{
		case EItemType::Consumable:
		{
			FConsumableItemData* ItemData = InventoryComp->GetConsumableItemInfo(ItemID);
			ItemName = ItemData->Name;
			ItemPrice = ItemData->Price;
			ItemInfo = ItemData->ItemTextInfo;
			ItemImageWidget->SetBrushFromTexture(ItemData->ItemImage);
			break;
		}
		case EItemType::Equipment:
		{
			FEquipmentItemData* ItemData = InventoryComp->GetEquipmentItemInfo(ItemID);
			ItemName = ItemData->Name;
			ItemPrice = ItemData->Price;
			ItemInfo = ItemData->ItemTextInfo;
			ItemImageWidget->SetBrushFromTexture(ItemData->ItemImage);
			break;
		}
		default:
			break;
	}
}

void UInventorySlotWidget::ClearSlot()
{
	ItemID = 0;

	if (!OnInventoryItemChangedHandle.IsValid())
	{
		InventoryComp->OnInventoryItemChangedDelegate.Remove(OnInventoryItemChangedHandle);
	}

	ItemType = EItemType::None;
	Amount = 0;
	ItemName = "";
	ItemPrice = 0;
	ItemInfo = "";
	ItemImageWidget->SetBrushFromTexture(nullptr);
	ItemImageWidget->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));

	return;
}

UTexture2D* UInventorySlotWidget::GetItemImage()
{
	return Cast<UTexture2D>(ItemImageWidget->GetBrush().GetResourceObject());
}
