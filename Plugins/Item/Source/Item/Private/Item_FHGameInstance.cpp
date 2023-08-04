// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHGameInstance.h"
#include "InventoryComponent.h"

void UItem_FHGameInstance::Init()
{
	Super::Init();
}

void UItem_FHGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UItem_FHGameInstance::AddItemToInventory(FInventoryItem Item)
{
	InventoryItemArray.Add(Item);
}

TArray<FInventoryItem>& UItem_FHGameInstance::GetInventoryItemArray()
{
	return InventoryItemArray;
}

int32 UItem_FHGameInstance::GetItemAmountInInventory(const int32& ItemID)
{
	for (auto& InventoryItem : GetInventoryItemArray())
	{
		if (InventoryItem.ID == ItemID)
		{
			return InventoryItem.Amount;
		}
	}

	return 0;
}

void UItem_FHGameInstance::TEST()
{
	for (auto a : InventoryItemArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d = %d"), a.ID, a.Amount);
	}
}
