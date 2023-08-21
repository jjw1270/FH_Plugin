// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotWidget.h"
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

	InitSlot();
}

void UEquipmentSlotWidget::InitSlot()
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
				EquipComp->OnEquipmentChangedDelegate.AddUObject(this, &UEquipmentSlotWidget::OnEquipmentChanged);
				return;
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(th_InitSlot, this, &UEquipmentSlotWidget::InitSlot, 0.1f, false);
}

void UEquipmentSlotWidget::OnEquipmentChanged(const EEquipmentType& EquipType, const int32& ItemID, const bool& bEquip)
{
	if (EquipType != EquipmentType)
	{
		return;
	}

	// if UnEquip
	if (!bEquip)
	{
		ClearSlot(ItemID);
		// inventory widget -> AddNewItemToSlot();
		PC->GetHUDWidget()->GetInventoryWidget()->AddNewItemToSlot(ItemID);
		return;
	}

	FEquipmentItemData* ItemData = InventoryComp->GetEquipmentItemInfo(ItemID);

	SetWidgetBindVariables(ItemData);
}

void UEquipmentSlotWidget::SetWidgetBindVariables(FEquipmentItemData* ItemData)
{
	ItemName = ItemData->Name;
	ItemPrice = ItemData->Price;
	ItemInfo = ItemData->ItemTextInfo;
	Image_Equip->SetBrushFromTexture(ItemData->ItemImage);
	Image_Equip->SetVisibility(ESlateVisibility::Visible);
}

void UEquipmentSlotWidget::ClearSlot(const int32& ItemID)
{
	Image_Equip->SetBrushFromTexture(nullptr);
	Image_Equip->SetVisibility(ESlateVisibility::Collapsed);
}
