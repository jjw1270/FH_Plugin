// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item_FHGameInstance.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UItem_FHGameInstance>(GetOwner()->GetGameInstance());
	ensureMsgf(GameInstance, TEXT("GameInstance is nullptr"));
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

}

void UInventoryComponent::AddItemToInventory(const int32& ItemID, const int32& Amount)
{
	// if Already Exist in Inventory, Add Amount
	for (auto& InventoryItem : GameInstance->GetInventoryItemArray())
	{
		if (InventoryItem.ID == ItemID)
		{
			InventoryItem.Amount += Amount;

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
				FString::Printf(TEXT("Loot Item ID = %d, Amount = %d"), InventoryItem.ID, InventoryItem.Amount));

			return;
		}
	}
	
	// else Make InventoryItem
	FInventoryItem InventoryItem = FInventoryItem(GetItemType(ItemID), ItemID, Amount);
	
	GameInstance->AddItemToInventory(InventoryItem);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
	FString::Printf(TEXT("Loot Item ID = %d, Amount = %d"), InventoryItem.ID, InventoryItem.Amount));

	return;
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

EItemType UInventoryComponent::GetItemType(const int32 ItemID)
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
