// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item_FHGameInstance.h"
#include "ItemInterface.h"
//Widget
#include "InventoryWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	UItem_FHGameInstance* GI = Cast<UItem_FHGameInstance>(GetOwner()->GetGameInstance());
	ensureMsgf(GI, TEXT("GI not Set"));

	InventoryItems = GI->GetInventoryItems();

	ensureMsgf(IsValid(InventoryWidgetClass), TEXT("InventoryWidgetClass is not set"));
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
}

FConsumableItemData* UInventoryComponent::GetConsumableItemInfo(const int32& ItemID)
{
	ensureMsgf(ConsumableItemDataTable, TEXT("ConsumableItemDataTable is nullptr"));

	return ConsumableItemDataTable->FindRow<FConsumableItemData>(*FString::FromInt(ItemID), TEXT(""), false);
}

FEquipmentItemData* UInventoryComponent::GetEquipmentItemInfo(const int32& ItemID)
{
	ensureMsgf(EquipmentItemDataTable, TEXT("EquipmentItemDataTable is nullptr"));

	return EquipmentItemDataTable->FindRow<FEquipmentItemData>(*FString::FromInt(ItemID), TEXT(""), false);
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
				InventoryWidget->UpdateItemToSlot(InventoryItem);

				return;
			}
		}
	}

	// else Make InventoryItem
	FInventoryItem* NewItem = new FInventoryItem(ItemType, ItemID, Amount);
	InventoryItems->Add(NewItem);
	InventoryWidget->AddItemToSlot(NewItem);

	return;
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
