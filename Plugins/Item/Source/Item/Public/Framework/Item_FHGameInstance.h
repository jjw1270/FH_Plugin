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
	UItem_FHGameInstance();

	//test
	//UFUNCTION(BlueprintCallable)
	//void TESTss();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentDungeonID;

public:
	UFUNCTION(BlueprintCallable)
	void SetCurrentDungeonID(const int32& DungeonID);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCurrentDungeonID() const { return CurrentDungeonID; }

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemDataManager> ItemDataManagerClass;

	UPROPERTY()
	class UItemDataManager* ItemDataManager;

public:
	FORCEINLINE class UItemDataManager* GetItemDataManager() const { return ItemDataManager; }

	// TMap<ItemData, ItemAmount>
	UPROPERTY()
	TMap<class UItemData*, int32> InventoryItems;

	// TMap<QuickSlotIdx, ItemData>
	UPROPERTY()
	TMap<int32, class UItemData*> QuickSlotItems;


	//// TMap<EquipType, ItemID>
	//UPROPERTY()
	//TMap<EEquipmentType, int32> Equiments;

public:
	FORCEINLINE TMap<class UItemData*, int32>& GetInventoryItems() { return InventoryItems; }

	FORCEINLINE TMap<int32, class UItemData*>& GetQuickSlotItems() { return QuickSlotItems; }

	//FORCEINLINE TMap<EEquipmentType, int32>* GetEquipments() { return &Equiments; }

};
