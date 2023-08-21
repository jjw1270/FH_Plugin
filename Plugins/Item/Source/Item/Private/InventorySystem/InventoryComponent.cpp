// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item.h"
#include "Item_FHGameInstance.h"
#include "Item_FHPlayerController.h"
#include "QuickSlotComponent.h"
#include "Item_FHPlayerState.h"
#include "EquipmentComponent.h"



UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void UInventoryComponent::Init()
{
	UItem_FHGameInstance* GI = Cast<UItem_FHGameInstance>(GetOwner()->GetGameInstance());
	CHECK_VALID(GI);

	AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(GetOwner());
	CHECK_VALID(PC);

	AItem_FHPlayerState* PS = PC->GetPlayerState<AItem_FHPlayerState>();
	CHECK_VALID(PS);

	InventoryItems = GI->GetInventoryItems();

	QuickSlotComp = PC->GetQuickSlotComp();
	CHECK_VALID(QuickSlotComp);

	EquipComp = PS->GetEquipmentComp();
	CHECK_VALID(EquipComp);
}

void UInventoryComponent::AddItemToInventory(const int32& ItemID, const int32& NewValue)
{
	// If Already Exist in Inventory, Add Amount
	//
	// If Item is Equipment Item, Value is Item Index
	// Else Value is Item Amount
	if (int32* PrevValue = InventoryItems->Find(ItemID))
	{
		*PrevValue += NewValue;

		if (ItemUpdateDelegate.IsBound())
		{
			ItemUpdateDelegate.Broadcast(ItemID, NewValue);
		}
		return;
	}

	// else Make New InventoryItem
	InventoryItems->Add(ItemID, NewValue);

	if (ItemUpdateDelegate.IsBound())
	{
		ItemUpdateDelegate.Broadcast(ItemID, NewValue);
	}
}

void UInventoryComponent::RemoveItemFromInventory(const int32& ItemID, const int32& NewValue)
{
	if (int32* PrevValue = InventoryItems->Find(ItemID))
	{
		*PrevValue -= NewValue;

		if (*PrevValue <= 0)
		{
			InventoryItems->Remove(ItemID);
		}

		// BroadCast
		if (ItemUpdateDelegate.IsBound())
		{
			ItemUpdateDelegate.Broadcast(ItemID, *PrevValue);
		}
	}
}

EItemType UInventoryComponent::GetItemType(const int32& ItemID)
{
	EItemType Itemtype = EItemType::None;

	switch (ItemID / 1000)
	{
	case 1:
		Itemtype = EItemType::Consumable;
		break;
	case 2:
		Itemtype = EItemType::Equipment;
		break;
	default:
		break;
	}

	return Itemtype;
}

void UInventoryComponent::ManageItem(const int32& ItemID, const int32& NewValue)
{
	switch (GetItemType(ItemID))
	{
		case EItemType::Consumable:
			// Cant use Item Directly On Inventory
			// Just Register Item to QuickSlot
			// Check item is already in quick slot, if true, Unregister item
			// QuickSlotComp->

			/*if (ItemRegisterDelegate.IsBound())
			{
				ItemRegisterDelegate.Broadcast(ItemID, true, 0);
			}*/
			break;
		case EItemType::Equipment:
			EquipComp->Equip(ItemID);
			
			/*if (ItemRegisterDelegate.IsBound())
			{
				ItemRegisterDelegate.Broadcast(ItemID, true, *InventoryItems->Find(ItemID));
			}*/
			break;
		default:
			break;
	}
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
