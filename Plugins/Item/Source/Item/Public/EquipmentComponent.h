// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemType.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

/*
* <EquipmentItemID>
* Helmet : 21nn
* Upper  : 22nn
* Lower  : 23nn
* Shoes  : 24nn
* Weapon : 25nn
*/

DECLARE_MULTICAST_DELEGATE_TwoParams(FDele_Multi_EquipmentChange, const EEquipmentType&, const int32&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FDele_Multi_InventoryItemEquip, const int32&, bool);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITEM_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipmentComponent();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	class AItem_FHPlayerController* PC;

	TMap<EEquipmentType, int32>* Equipments;

public:
	FDele_Multi_EquipmentChange OnEquipmentChangedDelegate;

	FDele_Multi_InventoryItemEquip OnInventoryItemEquipDelegate;

protected:
	void InitEquipment();

public:
	FORCEINLINE TMap<EEquipmentType, int32>* GetEquipments() { return Equipments; }

	void Equip(const int32& ItemID);

	void UnEquip(EEquipmentType EquipType);

	EEquipmentType GetEquipmentType(const int32& ItemID);

protected:
	bool IsEquipmentsHasThisEquipmentType();


};
