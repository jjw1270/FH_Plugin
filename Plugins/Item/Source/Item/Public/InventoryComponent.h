// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemType.h"
#include "InventoryComponent.generated.h"

/** Delegate called when an inventory item changes */
DECLARE_MULTICAST_DELEGATE_OneParam(FDele_Multi_OneParam, const int32&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	class AItem_FHPlayerController* PC;

// UI
public:
	UFUNCTION(BlueprintCallable)
	void InventoryUI();

	FDele_Multi_OneParam OnInventoryItemChangedDelegate;

protected:
	bool bIsInventoryUIOpen;
	
// Inventory
private:
	TMap<int32, int32>* InventoryItems;

public:
	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(const int32& ItemID, const int32& Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromInventory(const int32& ItemID, const int32& Amount);

	FORCEINLINE TMap<int32, int32>* GetInventoryItems() { return InventoryItems; }

	EItemType GetItemType(const int32& ItemID);

	void UseQuickSlotItem(const int32& QuickSlotNum);

	UFUNCTION(BlueprintCallable)
	void UseItem(const int32& ItemID);

	UPROPERTY(BlueprintReadWrite)
	bool bCanUseItem;

protected:
	void UseConsumableItem(const int32& ItemID);

	void EquipmentItem(const int32& ItemID);

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
