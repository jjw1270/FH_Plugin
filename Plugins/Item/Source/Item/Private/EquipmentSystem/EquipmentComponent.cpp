// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "Item.h"
#include "ItemData.h"
#include "ItemDataManager.h"
#include "Item_FHPlayerState.h"
#include "InventoryComponent.h"
#include "Item_FHGameInstance.h"
#include "Item_FHPlayerController.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	InitComponent();
}

void UEquipmentComponent::InitComponent()
{
	UItem_FHGameInstance* GI = Cast<UItem_FHGameInstance>(GetOwner()->GetGameInstance());
	CHECK_VALID(GI);

	AItem_FHPlayerState* PS = GetOwner<AItem_FHPlayerState>();
	CHECK_VALID(PS);

	AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(PS->GetPlayerController());
	CHECK_VALID(PC);

	EquipmentItems = GI->GetEquipments();

	InventoryComp = PC->GetInventoryComp();
	CHECK_VALID(InventoryComp);
}

void UEquipmentComponent::Equip(class UItemData* NewItemData)
{
	EItemType NewItemType = NewItemData->GetItemType();
	FArmorItemData NewArmorData;

	// Check Already Equipped at SameEquipType
	// if True : UnEquip Equipped Item
	for (auto& EquippedItem : EquipmentItems)
	{
		if (NewItemType != EquippedItem->GetItemType())
		{
			continue;
		}

		//Weapon Item
		if (NewItemType == EItemType::Weapon)
		{
			UnEquip(EquippedItem);
			break;
		}

		// Armor Item
		if (!NewItemData->GetArmorData(NewArmorData))
		{
			return;
		}

		FArmorItemData EquippedArmorData;
		if (!NewItemData->GetArmorData(EquippedArmorData))
		{
			return;
		}

		if (NewArmorData.ArmorType == EquippedArmorData.ArmorType)
		{
			UnEquip(EquippedItem);
			break;
		}
	}

	//Equip Item
	EquipmentItems.Add(NewItemData);

	// boradcast
	if (NewItemType == EItemType::Weapon)
	{
		if (WeaponUpdateDelegate.IsBound())
		{
			WeaponUpdateDelegate.Broadcast(NewItemData, true);
		}
	}
	else if (NewItemType == EItemType::Armor)
	{
		if (ArmorUpdateDelegate.IsBound())
		{
			ArmorUpdateDelegate.Broadcast(NewArmorData.ArmorType, NewItemData, true);
		}
	}
}

void UEquipmentComponent::UnEquip(class UItemData* TargetItemData)
{
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

	EquipmentItems.Remove(TargetItemData);
}
