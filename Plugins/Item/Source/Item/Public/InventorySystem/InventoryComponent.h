// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemType.h"
#include "InventoryComponent.generated.h"

// Delegate called when an inventory item changes
// const int32& ItemID, const int32& ItemAmount
DECLARE_MULTICAST_DELEGATE_TwoParams(FDele_Multi_ItemUpdate, const int32&, const int32&);

// Delegate called when inventory item is registered
// const int32& ItemID, const bool& bIsRegist, const int32& ItemIdx
// Third Parameter is used for Cant Stackable Item(ex. Equipment Item) Idx Only!
DECLARE_MULTICAST_DELEGATE_ThreeParams(FDele_Multi_ItemRegister, const int32&, const bool&, const int32&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

protected:
	void Init();

protected:
	UPROPERTY()
	class UEquipmentComponent* EquipComp;

	UPROPERTY()
	class UQuickSlotComponent* QuickSlotComp;

// for UI delegate
public:
	FDele_Multi_ItemUpdate ItemUpdateDelegate;

	FDele_Multi_ItemRegister ItemRegisterDelegate;
	
// Inventory
private:
	TMap<int32, int32>* InventoryItems;

public:
	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(const int32& ItemID, const int32& NewValue);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromInventory(const int32& ItemID, const int32& NewValue);

	FORCEINLINE TMap<int32, int32>* GetInventoryItems() { return InventoryItems; }

	EItemType GetItemType(const int32& ItemID);

	// void UseQuickSlotItem(const int32& QuickSlotNum);

	UFUNCTION(BlueprintCallable)
	void ManageItem(const int32& ItemID, const int32& NewValue);

//Item DataTables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* ConsumableItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* EquipmentItemDataTable;

public:
	struct FConsumableItemData* GetConsumableItemInfo(const int32& ItemID);

	struct FEquipmentItemData* GetEquipmentItemInfo(const int32& ItemID);

};
