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
	EItemType Type = EItemType::None;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ID = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Amount = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* ConsumableItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* EquipmentItemDataTable;

protected:
	UPROPERTY()
	class UItem_FHGameInstance* GameInstance;

protected:

	EItemType GetItemType(const int32 ItemID);

public:
	UFUNCTION(BlueprintCallable)
	void AddItemToInventory(const int32& ItemID, const int32& Amount);

	UFUNCTION(BlueprintCallable)
	FConsumableItemData* GetConsumableItemInfo(const int32& ItemID);

	UFUNCTION(BlueprintCallable)
	FEquipmentItemData* GetEquipmentItemInfo(const int32& ItemID);

};
