// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ItemType.h"
#include "Item_FHGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UItem_FHGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	// TMap<ItemID, ItemAmount>
	UPROPERTY()
	TMap<int32, int32> InventoryItems;

	// TMap<EquipType, ItemID>
	UPROPERTY()
	TMap<EEquipmentType, int32> Equiments;

public:
	FORCEINLINE TMap<int32, int32>* GetInventoryItems() { return &InventoryItems; }

	FORCEINLINE TMap<EEquipmentType, int32>* GetEquipments() { return &Equiments; }

};
