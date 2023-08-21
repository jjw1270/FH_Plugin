// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "Item.h"
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

	Init();
}

void UEquipmentComponent::Init()
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

void UEquipmentComponent::Equip(const int32& ItemID)
{
	//check already equipped at EquipType. if true : unequip prevEquipment
	EEquipmentType EquipType = GetEquipmentType(ItemID);
	
	// if already has equipped item, unequip first
	if (EquipmentItems->Contains(EquipType))
	{
		int32* PrevItemID = EquipmentItems->Find(EquipType);
		UnEquip(EquipType, *PrevItemID);
	}

	EquipmentItems->Add(EquipType, ItemID);

	if (EquipmentChangedDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("A"));
		EquipmentChangedDelegate.Broadcast(EquipType, ItemID, true);
	}
}

void UEquipmentComponent::UnEquip(EEquipmentType EquipType, const int32& ItemID)
{
	if (!EquipmentItems->Contains(EquipType))
	{
		return;
	}

	EquipmentItems->Remove(EquipType);
	
	// return item to inventory

	InventoryComp->AddItemToInventory(ItemID, 1);

	if (EquipmentChangedDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("B"));
		EquipmentChangedDelegate.Broadcast(EquipType, ItemID, false);
	}
}

EEquipmentType UEquipmentComponent::GetEquipmentType(const int32& ItemID)
{
	EEquipmentType EquipmentType = EEquipmentType::None;

	switch ((ItemID - 2000)/100)
	{
	case 1:
		EquipmentType = EEquipmentType::Helmet;
		break;
	case 2:
		EquipmentType = EEquipmentType::Upper;
		break;
	case 3:
		EquipmentType = EEquipmentType::Lower;
		break;
	case 4:
		EquipmentType = EEquipmentType::Shoes;
		break;
	case 5:
		EquipmentType = EEquipmentType::Weapon;
		break;
	default:
		break;
	}

	return EquipmentType;
}
