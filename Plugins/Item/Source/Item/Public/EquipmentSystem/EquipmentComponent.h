// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemType.h"
#include "EquipmentComponent.generated.h"

/*
* <EquipmentItemID>
* Helmet : 21nn
* Upper  : 22nn
* Lower  : 23nn
* Shoes  : 24nn
* Weapon : 25nn
*/

DECLARE_MULTICAST_DELEGATE_ThreeParams(FDele_Multi_EquipmentChange, const EEquipmentType&, const int32&, const bool&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITEM_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipmentComponent();

protected:
	virtual void BeginPlay() override;

protected:
	void Init();

protected:
	UPROPERTY()
	class UInventoryComponent* InventoryComp;

protected:
	TMap<EEquipmentType, int32>* EquipmentItems;

public:
	FDele_Multi_EquipmentChange EquipmentChangedDelegate;

public:
	FORCEINLINE TMap<EEquipmentType, int32>* GetEquipments() { return EquipmentItems; }

	void Equip(const int32& ItemID);

	void UnEquip(EEquipmentType EquipType, const int32& ItemID);

	EEquipmentType GetEquipmentType(const int32& ItemID);

protected:
	// bool IsEquipmentsHasThisEquipmentType();


};
