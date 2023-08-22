// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item.h"
#include "ItemDataManager.h"
#include "Item_FHGameInstance.h"
#include "Item_FHPlayerController.h"
#include "Item_FHPlayerState.h"
#include "QuickSlotComponent.h"
#include "EquipmentComponent.h"

#include "ItemData.h"
#include "Math/UnrealMathUtility.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	if (ItemDataManagerClass)
	{
		ItemDataManager = NewObject<UItemDataManager>(this, ItemDataManagerClass);
	}
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
	UItemData* NewItem;

	FWeaponItemData TempWeaponItemData;
	FArmorItemData TempArmorItemData;
	FConsumableItemData TempConsumableItemData;

	switch (NewItemType)
	{
	case EItemType::Weapon:
		if (!ItemDataManager->GetWeaponItemInfo(NewItemID, TempWeaponItemData))
		{
			UE_LOG(LogTemp, Error, TEXT("Data in WeaponItemDataTable is Missing"));
			return;
		}

		NewItem = NewObject<UItemData>(this, UItemData::StaticClass());
		if (NewItem)
		{
			NewItem->SetWeaponData(TempWeaponItemData, MakeUniqueID());
		}

		InventoryItems.Add(NewItem, NewAmount);

		if (ItemUpdateDelegate.IsBound())
		{
			ItemUpdateDelegate.Broadcast(NewItem, NewAmount);
		}

		return;
	case EItemType::Armor:
		if (!ItemDataManager->GetArmorItemInfo(NewItemID, TempArmorItemData))
		{
			UE_LOG(LogTemp, Error, TEXT("Data in ArmorItemDataTable is Missing"));
			return;
		}

		NewItem = NewObject<UItemData>(this, UItemData::StaticClass());
		if (NewItem)
		{
			NewItem->SetArmorData(TempArmorItemData, MakeUniqueID());
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
			if (BaseItemData.ID == NewItemID)
			{
				MyItem.Value += NewAmount;

				if (ItemUpdateDelegate.IsBound())
				{
					ItemUpdateDelegate.Broadcast(MyItem.Key, NewAmount);
				}

				return;
			}
		}

		// else Make New Item
		if (!ItemDataManager->GetConsumableItemInfo(NewItemID, TempConsumableItemData))
		{
			UE_LOG(LogTemp, Error, TEXT("Data in ConsumableItemDataTable is Missing"));
			return;
		}

		NewItem = NewObject<UItemData>(this, UItemData::StaticClass());
		if (NewItem)
		{
			NewItem->SetConsumableData(TempConsumableItemData);
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

void UInventoryComponent::RemoveItemFromInventory(class UItemData* ItemData, const int32& Amount)
{
	for (auto& MyItem : InventoryItems)
	{
		if (MyItem.Key == ItemData)
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

void UInventoryComponent::ManageItem(class UItemData* TargetItemData, const int32& TargetItemAmount)
{
	FBaseItemData BaseItemData;
	if (!TargetItemData->GetBaseData(BaseItemData))
	{
		return;
	}

	// Consumable Items
	if (BaseItemData.Type == EItemType::Consumable)
	{
		// Check QuickSlot is empty
		int32 QuickSlotIndex = QuickSlotComp->GetEmptyQuickSlotSlotIndex();
		// If QuickSlot is Full, Do Nothing
		if (QuickSlotIndex < 0)
		{
			return;
		}

		QuickSlotComp->SetItemToQuickSlot(QuickSlotIndex, TargetItemData, TargetItemAmount);

		return;
	}

	// Weapon, Armor Items
	if (BaseItemData.Type == EItemType::Weapon || BaseItemData.Type == EItemType::Armor)
	{
		EquipComp->Equip(TargetItemData);
	}
}
