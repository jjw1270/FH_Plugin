// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "Item_FHGameInstance.h"
#include "Item_FHPlayerController.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	UItem_FHGameInstance* GI = Cast<UItem_FHGameInstance>(GetOwner()->GetGameInstance());
	check(GI);

	Equipments = GI->GetEquipments();
	check(Equipments);

	//PC = Cast<AItem_FHPlayerController>(GetOwner());
	//check(PC);

	InitEquipment();
}

void UEquipmentComponent::InitEquipment()
{
	for (auto& Equip : *Equipments)
	{

	}
	// if GI has Equipments
}

void UEquipmentComponent::Equip(const int32& ItemID)
{
	//check already equipped at EquipType. if true : unequip prevEquipment
	EEquipmentType EquipType = GetEquipmentType(ItemID);
	
	if (Equipments->Contains(EquipType))
	{
		UnEquip(EquipType);
	}

	Equipments->Add(EquipType, ItemID);

	if (OnEquipmentChangedDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("A"));
		OnEquipmentChangedDelegate.Broadcast(EquipType, ItemID);
	}
}

void UEquipmentComponent::UnEquip(EEquipmentType EquipType)
{
	if (!Equipments->Contains(EquipType))
	{
		return;
	}

	Equipments->Remove(EquipType);

	if (OnEquipmentChangedDelegate.IsBound())
	{
		OnEquipmentChangedDelegate.Broadcast(EquipType, 0);
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
