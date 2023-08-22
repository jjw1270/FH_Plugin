// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDataManager.h"
#include "EquipmentComponent.generated.h"

/*
* <EquipmentItemID>
* Weapon : 2nnnn
* 
* <Armor>
* Helmet : 31nnn
* Upper  : 32nnn
* Gloves : 33nnn
* Lower  : 34nnn
* Shoes  : 35nnn
*/

// Delegate called when an Equipment Weapon Item Changed
// UItemData* ItemData, const bool& bEquip
DECLARE_MULTICAST_DELEGATE_TwoParams(FDele_Multi_WeaponUpdate, class UItemData*, const bool&);

// Delegate called when an Equipment Armor Item Changed
// const EArmorType& ArmorType, UItemData* ItemData, const bool& bEquip
DECLARE_MULTICAST_DELEGATE_ThreeParams(FDele_Multi_ArmorUpdate, const EArmorType&, class UItemData*, const bool&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITEM_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipmentComponent();

protected:
	virtual void BeginPlay() override;

protected:
	void InitComponent();

protected:
	UPROPERTY()
	class UInventoryComponent* InventoryComp;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UItemData*> EquipmentItems;

// UI Delegate
public:
	FDele_Multi_WeaponUpdate WeaponUpdateDelegate;

	FDele_Multi_ArmorUpdate ArmorUpdateDelegate;

public:
	UFUNCTION(BlueprintCallable)
	void Equip(class UItemData* NewItemData);

	UFUNCTION(BlueprintCallable)
	void UnEquip(class UItemData* TargetItemData);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<class UItemData*>& GetEquipmentItems() { return EquipmentItems; }

};
