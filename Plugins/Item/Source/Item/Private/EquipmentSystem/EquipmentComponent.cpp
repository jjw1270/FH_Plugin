// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "Item.h"
#include "ItemData.h"
#include "ItemDataManager.h"
#include "InventoryComponent.h"
#include "Item_FHGameInstance.h"
#include "Item_FHPlayerController.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetComponentTickEnabled(false);
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	InitComponent();
}


void UEquipmentComponent::InitComponent()
{
	AItem_FHPlayerController* PC = GetOwner<AItem_FHPlayerController>();
	CHECK_VALID(PC);

	GI = PC->GetGameInstance<UItem_FHGameInstance>();
	CHECK_VALID(GI);

	InventoryComp = PC->GetInventoryComp();
	CHECK_VALID(InventoryComp);
}

void UEquipmentComponent::ManageEquipment(UItemData* TargetItemData)
{
	// check Item is already regist
	// if true, UnEquip target Item
	EItemType ExistItemType;
	EArmorType ExistArmorType;
	if (IsItemExistInEquipmentSlot(TargetItemData, ExistItemType, ExistArmorType))
	{
		UnEquip(TargetItemData);

		return;
	}

	// else, EquipItem
	Equip(TargetItemData);
}

void UEquipmentComponent::Equip(class UItemData* NewItemData)
{
	EItemType NewItemType = NewItemData->GetItemType();
	FArmorItemData NewArmorData;
	NewItemData->GetArmorData(NewArmorData);

	// Check Already Equipped at Same EquipType
	// if True : UnEquip Equipped Item
	for (auto& EquippedItemData : *GI->GetEquipments())
	{
		// Weapon ItemType
		if (NewItemType == EItemType::Weapon && EquippedItemData->GetItemType() == NewItemType)
		{
			UnEquip(EquippedItemData);
			break;
		}

		// Armor ItemType
		FArmorItemData EquippedArmorData;
		if (EquippedItemData->GetArmorData(EquippedArmorData))
		{
			if (EquippedArmorData.ArmorType == NewArmorData.ArmorType)
			{
				UnEquip(EquippedItemData);
				break;
			}
		}
	}

	//Equip Item
	GI->GetEquipments()->Add(NewItemData);

	if (InventoryComp->ItemRegisterDelegate.IsBound())
	{
		InventoryComp->ItemRegisterDelegate.Broadcast(NewItemData, true);
	}

	// boradcast
	if (NewItemType == EItemType::Weapon)
	{
		if (WeaponUpdateDelegate.IsBound())
		{
			WeaponUpdateDelegate.Broadcast(NewItemData, true);
		}

		return;
	}
	else if (NewItemType == EItemType::Armor)
	{
		if (ArmorUpdateDelegate.IsBound())
		{
			ArmorUpdateDelegate.Broadcast(NewArmorData.ArmorType, NewItemData, true);
		}

		return;
	}
}

void UEquipmentComponent::UnEquip(class UItemData* TargetItemData)
{
	if (InventoryComp->ItemRegisterDelegate.IsBound())
	{
		InventoryComp->ItemRegisterDelegate.Broadcast(TargetItemData, false);
	}

	EItemType TargetItemType = TargetItemData->GetItemType();

	if (TargetItemType == EItemType::Weapon)
	{
		// boradcast
		if (WeaponUpdateDelegate.IsBound())
		{
			WeaponUpdateDelegate.Broadcast(TargetItemData, false);
		}
	}
	else if (TargetItemType == EItemType::Armor)
	{
		FArmorItemData TargetArmorData;
		if (!TargetItemData->GetArmorData(TargetArmorData))
		{
			return;
		}

		// boradcast
		if (ArmorUpdateDelegate.IsBound())
		{
			ArmorUpdateDelegate.Broadcast(TargetArmorData.ArmorType, TargetItemData, false);
		}
	}

	GI->GetEquipments()->Remove(TargetItemData);
}

bool UEquipmentComponent::IsItemExistInEquipmentSlot(UItemData* TargetItemData, EItemType& OutItemType, EArmorType& OutArmorType)
{
	for (auto& MyEquipItem : *GI->GetEquipments())
	{
		if (MyEquipItem != TargetItemData)
		{
			continue;
		}

		OutItemType = MyEquipItem->GetItemType();

		// Check Item has ArmorData
		FArmorItemData MyEquipArmorItemData;
		if (MyEquipItem->GetArmorData(MyEquipArmorItemData))
		{
			OutArmorType = MyEquipArmorItemData.ArmorType;
		}
		else
		{
			OutArmorType = EArmorType::None;
		}

		return true;
	}

	return false;
}
