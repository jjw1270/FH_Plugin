// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData.h"
#include "Item.h"

UItemData::UItemData()
{
}

void UItemData::SetConsumableData(FConsumableItemData& NewConsumableData)
{
	ConsumableData = NewConsumableData;
}

void UItemData::SetWeaponData(FWeaponItemData& NewWeaponData, int32 NewUniqueID)
{
	WeaponData = NewWeaponData;

	UniqueID = NewUniqueID;
}

void UItemData::SetArmorData(FArmorItemData& NewArmorData, int32 NewUniqueID)
{
	ArmorData = NewArmorData;

	UniqueID = NewUniqueID;
}

bool UItemData::GetBaseData(FBaseItemData& OutData)
{
	if (ConsumableData.IsValid())
	{
		OutData = ConsumableData.BaseData;
		return true;
	}
	if (WeaponData.IsValid())
	{
		OutData = WeaponData.BaseData;
		return true;
	}
	if (ArmorData.IsValid())
	{
		OutData = ArmorData.BaseData;
		return true;
	}

	return false;;
}

EItemType UItemData::GetItemType()
{
	if (ConsumableData.IsValid())
	{
		return ConsumableData.BaseData.Type;
	}
	if (WeaponData.IsValid())
	{
		return WeaponData.BaseData.Type;
	}
	if (ArmorData.IsValid())
	{
		return ArmorData.BaseData.Type;
	}

	return EItemType::None;
}

bool UItemData::GetConsumableData(FConsumableItemData& OutData)
{
	if (!ConsumableData.IsValid())
	{
		return false;
	}

	OutData = ConsumableData;
	return true;
}

bool UItemData::GetWeaponData(FWeaponItemData& OutData)
{
	if (!WeaponData.IsValid())
	{
		return false;
	}

	OutData = WeaponData;
	return true;
}

bool UItemData::GetArmorData(FArmorItemData& OutData)
{
	if (!ArmorData.IsValid())
	{
		return false;
	}

	OutData = ArmorData;
	return true;
}
