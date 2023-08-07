// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* ConsumableItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* EquipmentItemDataTable;

public:
	struct FConsumableItemData* GetConsumableItemInfo(const int32& ItemID);

	struct FEquipmentItemData* GetEquipmentItemInfo(const int32& ItemID);

protected:
	TArray<FInventoryItem*>* InventoryItems;

public:
	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(const int32& ItemID, const int32& Amount);

	FORCEINLINE TArray<FInventoryItem*>* GetInventoryItems() { return InventoryItems; }

private:
	EItemType GetItemType(const int32& ItemID);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	class UInventoryWidget* InventoryWidget;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UInventoryWidget* GetInventoryWidget() { return InventoryWidget; }
};
