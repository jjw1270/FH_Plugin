// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotWidget.h"
#include "ItemData.h"
#include "Components/Image.h"
#include "Item_FHPlayerState.h"
#include "EquipmentComponent.h"
#include "Item_FHPlayerController.h"
#include "InventoryComponent.h"
#include "Item_HUDWidget.h"
#include "InventoryWidget.h"

void UEquipmentSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Image_BackIcon->SetBrushFromTexture(BackIconTexture);

	BindEquipmentCompEvents();
}

void UEquipmentSlotWidget::BindEquipmentCompEvents()
{
	PC = GetOwningPlayer<AItem_FHPlayerController>();

	if (IsValid(PC))
	{
		InventoryComp = PC->GetInventoryComp();

		AItem_FHPlayerState* PS = PC->GetPlayerState<AItem_FHPlayerState>();
		if (IsValid(PS))
		{
			EquipComp = PS->GetEquipmentComp();
			if (IsValid(EquipComp))
			{
				if (ItemType == EItemType::Weapon)
				{
					EquipComp->WeaponUpdateDelegate.AddUObject(this, &UEquipmentSlotWidget::OnWeaponUpdate);
					return;
				}
				if (ItemType == EItemType::Armor)
				{
					EquipComp->ArmorUpdateDelegate.AddUObject(this, &UEquipmentSlotWidget::OnArmorUpdate);
					return;
				}
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, this, &UEquipmentSlotWidget::BindEquipmentCompEvents, 0.1f, false);
}

void UEquipmentSlotWidget::OnWeaponUpdate(class UItemData* UpdateItemData, const bool& bEquip)
{
	// if UnEquip
	if (!bEquip)
	{
		ClearSlot();
		return;
	}

	SetSlot(UpdateItemData);
}

void UEquipmentSlotWidget::OnArmorUpdate(const EArmorType& UpdateArmorType, UItemData* UpdateItemData, const bool& bEquip)
{
	//Check ArmorType
	if (ArmorType != UpdateArmorType)
	{
		return;
	}

	// if UnEquip
	if (!bEquip)
	{
		ClearSlot();
		return;
	}
	
	SetSlot(UpdateItemData);
}

void UEquipmentSlotWidget::SetSlot(class UItemData* UpdateItemData)
{
	EquippedItemData = UpdateItemData;

	FBaseItemData EquippedBaseItemData;
	if (!EquippedItemData->GetBaseData(EquippedBaseItemData))
	{
		return;
	}

	Image_Equip->SetBrushFromTexture(EquippedBaseItemData.Icon2D);
	Image_Equip->SetVisibility(ESlateVisibility::Visible);
}

void UEquipmentSlotWidget::ClearSlot()
{
	EquippedItemData = nullptr;

	Image_Equip->SetBrushFromTexture(nullptr);
	Image_Equip->SetVisibility(ESlateVisibility::Collapsed);
}
