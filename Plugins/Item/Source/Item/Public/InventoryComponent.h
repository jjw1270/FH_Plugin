// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

// Inventory Item Struct
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

DECLARE_MULTICAST_DELEGATE_OneParam(FDele_Multi_OneParam_pFInventoryItem, struct FInventoryItem*);

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

	FDele_Multi_OneParam_pFInventoryItem Fuc_Dele_OnInventoryItemChanged;

protected:
	bool bIsInventoryUIOpen;
	
// Inventory
public:
	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(const int32& ItemID, const int32& Amount);

	FORCEINLINE TArray<FInventoryItem*>* GetInventoryItems() { return InventoryItems; }

	int32 RemoveItemFromInventory(const FInventoryItem* InventoryItem, int32 Amount);

protected:
	EItemType GetItemType(const int32& ItemID);

private:
	TArray<FInventoryItem*>* InventoryItems;

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
