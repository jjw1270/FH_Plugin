// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item_FHGameInstance.h"
#include "Item_FHPlayerController.h"
#include "ItemInterface.h"
#include "Item_HUDWidget.h"
#include "InventoryWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
		for (auto InventoryItem : *InventoryItems)
		{
			if (InventoryItem->ID == ItemID)
			{
				InventoryItem->Amount += Amount;
				// BroadCast
				if (Fuc_Dele_OnInventoryItemChanged.IsBound())
				{
					Fuc_Dele_OnInventoryItemChanged.Broadcast(InventoryItem);
				}
				return;
			}
		}
	}

	// else Make InventoryItem
	FInventoryItem* NewItem = new FInventoryItem(ItemType, ItemID, Amount);
	InventoryItems->Add(NewItem);
	UE_LOG(LogTemp, Warning, TEXT("A"));
	PC->GetHUDWidget()->GetInventoryWidget()->AddNewItemToSlot(NewItem);

	return;
}

int32 UInventoryComponent::RemoveItemFromInventory(const FInventoryItem* InventoryItem, int32 Amount)
{
	for (auto Item : *InventoryItems)
	{
		if (Item == InventoryItem)
		{
			Item->Amount -= Amount;

			if (Item->Amount <= 0)
			{
				InventoryItems->Remove(Item);
				delete Item;
				UE_LOG(LogTemp, Warning, TEXT("B"));
				// Need to change FInventoryItem* to week_Ptr to fix!
				return 0;
			}

			return Item->Amount;
		}
	}

	return 0;
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
