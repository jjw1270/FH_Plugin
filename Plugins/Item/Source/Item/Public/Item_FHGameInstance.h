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

protected:
	TArray<struct FInventoryItem*> InventoryItems;

public:
	FORCEINLINE TArray<struct FInventoryItem*>* GetInventoryItems() { return &InventoryItems; }

};
