// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item_FHGameInstance.h"
#include "Item_FHPlayerController.h"
#include "ItemInterface.h"
#include "Item_HUDWidget.h"
#include "InventoryWidget.h"
#include "QuickSlotWidget.h"
#include "QuickSlotSlotWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::TestInventory()
{
	for (auto& a : *InventoryItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), a.Value);
	}
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UItem_FHGameInstance* GI = Cast<UItem_FHGameInstance>(GetOwner()->GetGameInstance());
	check(GI);

	InventoryItems = GI->GetInventoryItems();
	check(InventoryItems);

	PC = Cast<AItem_FHPlayerController>(GetOwner());
	check(PC);
}

void UInventoryComponent::InventoryUI()
{
	//Open Inventory UI
	if (!bIsInventoryUIOpen)
	{
		bIsInventoryUIOpen = true;
		PC->GetHUDWidget()->GetInventoryWidget()->SetVisibility(ESlateVisibility::Visible);
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeGameAndUI());
	}
	//Close Inventory UI
	else
	{
		bIsInventoryUIOpen = false;
		PC->GetHUDWidget()->GetInventoryWidget()->SetVisibility(ESlateVisibility::Collapsed);
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void UInventoryComponent::AddItemToInventory(const int32& ItemID, const int32& Amount)
{
	EItemType ItemType = GetItemType(ItemID);
	if (ItemType == EItemType::Consumable)
	{
		// if Already Exist in Inventory, Add Amount
		if (int32* PrevAmount = InventoryItems->Find(ItemID))
		{
			*PrevAmount += Amount;

			// BroadCast
			if (OnInventoryItemChangedDelegate.IsBound())
			{
				OnInventoryItemChangedDelegate.Broadcast(ItemID);
			}
			return;
		}
	}

	// else Make New InventoryItem
	InventoryItems->Add(ItemID, Amount);
	PC->GetHUDWidget()->GetInventoryWidget()->AddNewItemToSlot(ItemID);

	return;
}

void UInventoryComponent::RemoveItemFromInventory(const int32& ItemID, const int32& Amount)
{
	if (int32* PrevAmount = InventoryItems->Find(ItemID))
	{
		*PrevAmount -= Amount;

		if (*PrevAmount <= 0)
		{
			InventoryItems->Remove(ItemID);
		}

		// BroadCast
		OnInventoryItemChangedDelegate.Broadcast(ItemID);
	}
}

EItemType UInventoryComponent::GetItemType(const int32& ItemID)
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

void UInventoryComponent::UseQuickSlotItem(const int32& QuickSlotNum)
{
	// find item in quick slot
	UQuickSlotSlotWidget* QuickSlotSlot = PC->GetHUDWidget()->GetQuickSlotWidget()->GetQuickSlotSlot(QuickSlotNum);
	if (!QuickSlotSlot)
	{
		return;
	}

	// use item
	UseItem(QuickSlotSlot->GetSlotItemID());
}

void UInventoryComponent::UseItem(const int32& ItemID)
{
	// first check can use item
	// can use only in dungeon, not on attacking, not on using item, ...
	if (!bCanUseItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not Use Items!"));
		return;
	}

	EItemType ItemType = GetItemType(ItemID);
	switch (ItemType)
	{
		case EItemType::Consumable:
			UseConsumableItem(ItemID);
			break;
		case EItemType::Equipment:
			EquipmentItem(ItemID);
			break;
		default:
			break;
	}
}

void UInventoryComponent::UseConsumableItem(const int32& ItemID)
{

}

void UInventoryComponent::EquipmentItem(const int32& ItemID)
{

}

FConsumableItemData* UInventoryComponent::GetConsumableItemInfo(const int32& ItemID)
{
	check(ConsumableItemDataTable);

	return ConsumableItemDataTable->FindRow<FConsumableItemData>(*FString::FromInt(ItemID), TEXT(""), false);
}

FEquipmentItemData* UInventoryComponent::GetEquipmentItemInfo(const int32& ItemID)
{
	check(EquipmentItemDataTable);

	return EquipmentItemDataTable->FindRow<FEquipmentItemData>(*FString::FromInt(ItemID), TEXT(""), false);
}
