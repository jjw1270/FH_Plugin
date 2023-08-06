// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "ItemInterface.h"
#include "Item_FHGameInstance.h"

void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GameInstance = Cast<UItem_FHGameInstance>(GetGameInstance());
	ensureMsgf(GameInstance, TEXT("GameInstance is nullptr"));
}

void UInventorySlotWidget::SetItemDataToSlot(const struct FInventoryItem& InventoryItem)
{
	Amount = InventoryItem.Amount;

	switch (InventoryItem.Type)
	{
		case EItemType::Consumable:
		{
			FConsumableItemData* ItemData = GameInstance->GetConsumableItemInfo(InventoryItem.ID);
			ItemType = FString("Consumable");
			ItemName = ItemData->Name;
			ItemPrice = ItemData->Price;
			ItemInfo = ItemData->ItemTextInfo;
			Image = ItemData->ItemImage;
			break;
		}
		case EItemType::Equipment:
		{
			FEquipmentItemData* ItemData = GameInstance->GetEquipmentItemInfo(InventoryItem.ID);
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

