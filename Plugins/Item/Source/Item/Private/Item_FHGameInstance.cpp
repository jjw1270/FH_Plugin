// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHGameInstance.h"
#include "ItemInterface.h"

void UItem_FHGameInstance::Init()
{
	Super::Init();
}

void UItem_FHGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UItem_FHGameInstance::AddItemToInventory(const int32& ItemID, const int32& Amount)
{
	EItemType ItemType = GetItemType(ItemID);

	if (ItemType == EItemType::Consumable)
	{
		// if Already Exist in Inventory, Add Amount
		for (auto& InventoryItem : InventoryItemArray)
		{
			if (InventoryItem.ID == ItemID)
			{
				InventoryItem.Amount += Amount;

				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
					FString::Printf(TEXT("Loot Item ID = %d, Amount = %d"), InventoryItem.ID, InventoryItem.Amount));

				return;
			}
		}
	}

	// else Make InventoryItem
	FInventoryItem InventoryItem = FInventoryItem(ItemType, ItemID, Amount);

	InventoryItemArray.Add(InventoryItem);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
		FString::Printf(TEXT("New Loot Item ID = %d, Amount = %d"), InventoryItem.ID, InventoryItem.Amount));

	return;
}

TArray<FInventoryItem>& UItem_FHGameInstance::GetInventoryItemArray()
{
	return InventoryItemArray;
}

int32 UItem_FHGameInstance::GetItemAmountInInventory(const int32& ItemID)
{
	if (GetItemType(ItemID) == EItemType::Equipment)
	{
		return 1;
	}

	for (const auto& InventoryItem : InventoryItemArray)
	{
		if (InventoryItem.ID == ItemID)
		{
			return InventoryItem.Amount;
		}
	}

	return 0;
}

FConsumableItemData* UItem_FHGameInstance::GetConsumableItemInfo(const int32& ItemID)
{
	ensureMsgf(ConsumableItemDataTable, TEXT("ConsumableItemDataTable is nullptr"));

	return ConsumableItemDataTable->FindRow<FConsumableItemData>(*FString::FromInt(ItemID), TEXT(""), false);
}

FEquipmentItemData* UItem_FHGameInstance::GetEquipmentItemInfo(const int32& ItemID)
{
	ensureMsgf(EquipmentItemDataTable, TEXT("EquipmentItemDataTable is nullptr"));

	return EquipmentItemDataTable->FindRow<FEquipmentItemData>(*FString::FromInt(ItemID), TEXT(""), false);
}

EItemType UItem_FHGameInstance::GetItemType(const int32& ItemID)
{
	EItemType Itemtype;

	switch (ItemID / 1000)
	{
	case 1:
		Itemtype = EItemType::Consumable;
		break;
	case 2:
		Itemtype = EItemType::Equipment;
		break;
	default:
		Itemtype = EItemType::None;
		break;
	}

	return Itemtype;
}
