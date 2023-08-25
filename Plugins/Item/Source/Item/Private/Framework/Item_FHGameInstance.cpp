// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHGameInstance.h"
#include "ItemData.h"

void UItem_FHGameInstance::TESTss()
{
	UE_LOG(LogTemp, Warning, TEXT("<<<<Inventory Items>>>>"));
	for (const auto& Item : InventoryItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : %d [%d]"), *Item.Key->GetBaseData().Name, Item.Value, Item.Key->GetUniqueID());
	}
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------"));

	UE_LOG(LogTemp, Warning, TEXT("<<<<QuickSlot Items>>>>"));
	for (const auto& Item : QuickSlotItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("Index[%d] : %s"), Item.Key, *Item.Value->GetBaseData().Name);
	}
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------"));

	UE_LOG(LogTemp, Warning, TEXT("<<<<Equipment Items>>>>"));
	for (const auto& Item : EquipmentItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s [%d]"), *Item->GetBaseData().Name, Item->GetUniqueID());
	}
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------"));

}
