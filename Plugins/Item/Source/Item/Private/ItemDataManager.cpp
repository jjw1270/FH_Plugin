// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataManager.h"


FConsumableItemData* UItemDataManager::GetConsumableItemInfo(const int32& ItemID)
{
	check(ConsumableItemDataTable);

	return ConsumableItemDataTable->FindRow<FConsumableItemData>(*FString::FromInt(ItemID), TEXT(""), false);
}

FEquipmentItemData* UItemDataManager::GetEquipmentItemInfo(const int32& ItemID)
{
	check(EquipmentItemDataTable);

	return EquipmentItemDataTable->FindRow<FEquipmentItemData>(*FString::FromInt(ItemID), TEXT(""), false);
}
