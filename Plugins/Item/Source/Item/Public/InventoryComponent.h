// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemInterface.h"
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
	FInventoryItem() {};

	FInventoryItem(EItemType Type, int32 ID, int32 Amount)
		: Type(Type), ID(ID), Amount(Amount) {};
	
public:
	UPROPERTY(VisibleAnywhere)
	EItemType Type;
	
	UPROPERTY(VisibleAnywhere)
	int32 ID;

	UPROPERTY(VisibleAnywhere)
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

	virtual void InitializeComponent() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<FInventoryItem> InventoryItemArray;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* ConsumableItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* EquipmentItemDataTable;

protected:
	FConsumableItemData* GetConsumableItemInfo(const int32& ItemID);

	FEquipmentItemData* GetEquipmentItemInfo(const int32& ItemID);


public:
	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(const int32& ItemID, const int32& Amount);

	int32 GetItemAmountInInventory(const int32& ItemID);

};
