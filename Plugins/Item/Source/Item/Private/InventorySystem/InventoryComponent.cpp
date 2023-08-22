// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item.h"
#include "Item_FHGameInstance.h"
#include "Item_FHPlayerController.h"
#include "Item_FHPlayerState.h"
#include "QuickSlotComponent.h"
#include "EquipmentComponent.h"

#include "ItemData.h"
#include "ItemDataManager.h"
#include "Math/UnrealMathUtility.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitComponent();
}

void UInventoryComponent::InitComponent()
{
	UItem_FHGameInstance* GI = Cast<UItem_FHGameInstance>(GetOwner()->GetGameInstance());
	CHECK_VALID(GI);

	AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(GetOwner());
	CHECK_VALID(PC);

	AItem_FHPlayerState* PS = PC->GetPlayerState<AItem_FHPlayerState>();
	CHECK_VALID(PS);

	ItemDataManager = GI->GetItemDataManager();

	InventoryItems = GI->GetInventoryItems();

	QuickSlotComp = PC->GetQuickSlotComp();
	CHECK_VALID(QuickSlotComp);

	EquipComp = PS->GetEquipmentComp();
	CHECK_VALID(EquipComp);
}

void UInventoryComponent::AddItemToInventory(const int32& NewItemID, const int32& NewAmount)
{
	// If Item is Equipment Item, Add new item with unique id
	// else, Already Exist in Inventory, Add Amount

	EItemType NewItemType = ItemDataManager->GetItemType(NewItemID);

	switch (NewItemType)
	{
	case EItemType::Weapon:
		FWeaponItemData WeaponItemData;
		if (!ItemDataManager->GetWeaponItemInfo(NewItemID, WeaponItemData))
		{
			UE_LOG(LogTemp, Error, TEXT("Data in WeaponItemDataTable is Missing"));
			return;
		}

		UItemData* NewItem = NewObject<UItemData>(this, UItemData::StaticClass());
		if (NewItem)
		{
			NewItem->SetWeaponData(WeaponItemData, MakeUniqueID());
		}

		InventoryItems.Add(NewItem, NewAmount);

		if (ItemUpdateDelegate.IsBound())
		{
			ItemUpdateDelegate.Broadcast(NewItem, NewAmount);
		}

		return;
	case EItemType::Armor:
		FArmorItemData ArmorItemData;
		if (!ItemDataManager->GetArmorItemInfo(NewItemID, ArmorItemData))
		{
			UE_LOG(LogTemp, Error, TEXT("Data in ArmorItemDataTable is Missing"));
			return;
		}

		UItemData* NewItem = NewObject<UItemData>(this, UItemData::StaticClass());
		if (NewItem)
		{
			NewItem->SetArmorData(ArmorItemData, MakeUniqueID());
		}

		InventoryItems.Add(NewItem, NewAmount);

		if (ItemUpdateDelegate.IsBound())
		{
			ItemUpdateDelegate.Broadcast(NewItem, NewAmount);
		}

		return;
	case EItemType::Consumable:
		for (auto& MyItem : InventoryItems)
		{
			FBaseItemData BaseItemData;
			if (!MyItem.Key->GetBaseData(BaseItemData))
			{
				UE_LOG(LogTemp, Error, TEXT("CRITICAL ERROR in Inventory!!!"));
				return;
			}

			// Check Item is already in Inventory and add Amount if true
			if (BaseItemData.ItemID == NewItemID)
			{
				MyItem.Value += NewAmount;

				if (ItemUpdateDelegate.IsBound())
				{
					ItemUpdateDelegate.Broadcast(NewItem, NewAmount);
				}

				return;
			}
		}

		// else Make New Item
		FConsumableItemData ConsumableItemData;
		if (!ItemDataManager->GetConsumableItemInfo(NewItemID, ConsumableItemData))
		{
			UE_LOG(LogTemp, Error, TEXT("Data in ConsumableItemDataTable is Missing"));
			return;
		}

		UItemData* NewItem = NewObject<UItemData>(this, UItemData::StaticClass());
		if (NewItem)
		{
			NewItem->SetConsumableData(ConsumableItemData);
		}

		InventoryItems.Add(NewItem, NewAmount);

		if (ItemUpdateDelegate.IsBound())
		{
			ItemUpdateDelegate.Broadcast(NewItem, NewAmount);
		}

		return;
	case EItemType::Others:
		break;
	default:
		break;
	}
}

void UInventoryComponent::RemoveItemFromInventory(const int32& ItemID, const int32& UniqueID, const int32& Amount)
{
	for (auto& MyItem : InventoryItems)
	{
		FBaseItemData BaseItemData;
		if (!MyItem.Key->GetBaseData(BaseItemData))
		{
			UE_LOG(LogTemp, Error, TEXT("CRITICAL ERROR in Inventory!!!"));
			return;
		}

		if (BaseItemData.ItemID == ItemID)
		{
			MyItem.Value -= Amount;

			// BroadCast
			if (ItemUpdateDelegate.IsBound())
			{
				ItemUpdateDelegate.Broadcast(MyItem.Key, MyItem.Value);
			}

			if (MyItem.Value <= 0)
			{
				InventoryItems.Remove(MyItem.Key);
			}

			return;
		}
	}
}

int32 UInventoryComponent::MakeUniqueID()
{
	while (true)
	{
		bool bSuccess = true;
		int32 TempID = FMath::RandRange(1001, 9999);

		for (auto& Item : InventoryItems)
		{
			if (Item.Key->GetUniqueID() == TempID)
			{
				bSuccess = false;
				break;
			}
		}

		if (bSuccess)
		{
			return TempID;
		}
	}
}

//void UInventoryComponent::ManageItem(const int32& ItemID, const int32& NewAmount)
//{
//	switch (GetItemType(ItemID))
//	{
//		case EItemType::Consumable:
//			// Cant use Item Directly On Inventory
//			// Just Register Item to QuickSlot
//			// Check item is already in quick slot, if true, Unregister item
//			// QuickSlotComp->
//
//			/*if (ItemRegisterDelegate.IsBound())
//			{
//				ItemRegisterDelegate.Broadcast(ItemID, true, 0);
//			}*/
//			break;
//		case EItemType::Equipment:
//			EquipComp->Equip(ItemID);
//			
//			/*if (ItemRegisterDelegate.IsBound())
//			{
//				ItemRegisterDelegate.Broadcast(ItemID, true, *InventoryItems->Find(ItemID));
//			}*/
//			break;
//		default:
//			break;
//	}
//}
