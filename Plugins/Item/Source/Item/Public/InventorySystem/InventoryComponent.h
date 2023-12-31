// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

// Delegate called when an inventory item changed
// UItemData* ItemData, const int32& ItemAmount
DECLARE_MULTICAST_DELEGATE_TwoParams(FDele_Multi_ItemUpdate, class UItemData*, const int32&);

// Delegate called when inventory item is registered
// UItemData* ItemData, const int32& UniqueID, const bool& bIsRegist
DECLARE_MULTICAST_DELEGATE_TwoParams(FDele_Multi_ItemRegister, class UItemData*, const bool&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

protected:
	void InitComponent();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Data)
	TSubclassOf<class UItemDataManager> ItemDataManagerClass;

protected:
	UPROPERTY()
	class UItem_FHGameInstance* GI;

	UPROPERTY()
	class UItemDataManager* ItemDataManager;

	UPROPERTY()
	class UEquipmentComponent* EquipComp;

	UPROPERTY()
	class UQuickSlotComponent* QuickSlotComp;
	
// UI delegate
public:
	FDele_Multi_ItemUpdate ItemUpdateDelegate;

	FDele_Multi_ItemRegister ItemRegisterDelegate;

// Inventory func
public:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddItemToInventory(const int32& NewItemID, const int32& NewAmount);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void RemoveItemFromInventory(class UItemData* ItemData, const int32& Amount);

	int32 MakeUniqueID();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void ManageItem(class UItemData* TargetItemData, const int32& TargetItemAmount);

protected:
	class UItemData* GetWeaponItemData(const int32& TargetItemID);

	class UItemData* GetArmorItemData(const int32& TargetItemID);

	class UItemData* GetConsumableItemData(const int32& TargetItemID);

};
