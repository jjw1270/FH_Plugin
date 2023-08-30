// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Item_FHGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UItem_FHGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	//test
	UFUNCTION(BlueprintCallable)
	void TESTss();

protected:
	virtual void Init() override;

	virtual void Shutdown() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CurrentDungeonID;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCurrentDungeonID(const int32& NewDungeonID) { CurrentDungeonID = NewDungeonID; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentDungeonID() const { return CurrentDungeonID; }

protected:
	// TMap<ItemData, ItemAmount>
	UPROPERTY()
	TMap<class UItemData*, int32> InventoryItems;

	// TMap<QuickSlotIdx, ItemData>
	UPROPERTY()
	TMap<int32, class UItemData*> QuickSlotItems;

	// TArray<ItemData>
	UPROPERTY()
	TArray<class UItemData*> EquipmentItems;

public:
	FORCEINLINE TMap<class UItemData*, int32>* GetInventoryItems() { return &InventoryItems; }

	FORCEINLINE TMap<int32, class UItemData*>* GetQuickSlotItems() { return &QuickSlotItems; }

	FORCEINLINE TArray<class UItemData*>* GetEquipments() { return &EquipmentItems; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FDefaultPlayerStats DefaultPlayerStats;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FDefaultPlayerStats& GetDefaultPlayerStats() { return DefaultPlayerStats; }
};
