// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Item_FHGameInstance.generated.h"

struct FInventoryItem;

/**
 * 
 */
UCLASS()
class ITEM_API UItem_FHGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	virtual void Init() override;

	virtual void Shutdown() override;

protected:
	UPROPERTY()
	TArray<FInventoryItem> InventoryItemArray;

public:
	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(FInventoryItem Item);

	UFUNCTION(BlueprintCallable)
	TArray<FInventoryItem>& GetInventoryItemArray();

	UFUNCTION(BlueprintCallable)
	int32 GetItemAmountInInventory(const int32& ItemID);

	
	void TEST();
};
