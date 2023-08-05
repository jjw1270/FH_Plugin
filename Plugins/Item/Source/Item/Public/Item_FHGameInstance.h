// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Item_FHGameInstance.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None            UMETA(DisplayName = "None"),
	Consumable      UMETA(DisplayName = "Consumable"),
	Equipment       UMETA(DisplayName = "Equipment"),
};

USTRUCT(Atomic, BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	public:
	FInventoryItem()
	{
		Type = EItemType::None;
		ID = 0;
		Amount = 0;
	};

	FInventoryItem(EItemType Type, int32 ID, int32 Amount)
		: Type(Type), ID(ID), Amount(Amount) {};

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EItemType Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Amount;
};

/**
 * 
 */
UCLASS()
class ITEM_API UItem_FHGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* ConsumableItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* EquipmentItemDataTable;

protected:
	UPROPERTY()
	TArray<FInventoryItem> InventoryItemArray;
	
protected:
	virtual void Init() override;

	virtual void Shutdown() override;

public:
	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(const int32& ItemID, const int32& Amount);

	TArray<FInventoryItem>& GetInventoryItemArray();

	UFUNCTION(BlueprintCallable)
	int32 GetItemAmountInInventory(const int32& ItemID);

	FConsumableItemData* GetConsumableItemInfo(const int32& ItemID);

	FEquipmentItemData* GetEquipmentItemInfo(const int32& ItemID);

private:
	EItemType GetItemType(const int32& ItemID);

};
