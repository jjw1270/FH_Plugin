// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "ItemInterface.h"
#include "InventoryComponent.h"
#include "Components/TextBlock.h"

void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Amount = 0;
}

void UInventorySlotWidget::AddItemAmount(int32 NewAmount)
{
	Amount = NewAmount;
}

void UInventorySlotWidget::SetItemDataToSlot(struct FInventoryItem* InventoryItem, class UInventoryComponent* InventoryComp)
{
	ItemInfoBox = InventoryComp->GetInventoryWidget()->GetItemInfoBox();

	SlotInventoryItem = InventoryItem;

	Amount = SlotInventoryItem->Amount;

	switch (SlotInventoryItem->Type)
	{
		case EItemType::Consumable:
		{
			FConsumableItemData* ItemData = InventoryComp->GetConsumableItemInfo(SlotInventoryItem->ID);
			ItemType = FString("Consumable");
			ItemName = ItemData->Name;
			ItemPrice = ItemData->Price;
			ItemInfo = ItemData->ItemTextInfo;
			Image = ItemData->ItemImage;
			break;
		}
		case EItemType::Equipment:
		{
			FEquipmentItemData* ItemData = InventoryComp->GetEquipmentItemInfo(SlotInventoryItem->ID);
			ItemType = FString("Equipment");
			ItemName = ItemData->Name;
			ItemPrice = ItemData->Price;
			ItemInfo = ItemData->ItemTextInfo;
			Image = ItemData->ItemImage;
			break;
		}
		default:
			break;
	}
}
