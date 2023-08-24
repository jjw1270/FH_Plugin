// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDataManager.h"
#include "EquipmentSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	void BindEquipmentCompEvents();

protected:
	UPROPERTY()
	class UEquipmentComponent* EquipComp;

	FTimerHandle InitTimerHandle;

	UPROPERTY()
	class UInventoryComponent* InventoryComp;

	UPROPERTY()
	class AItem_FHPlayerController* PC;

//Need Edit Vars
protected:
	UPROPERTY(EditAnywhere)
	UTexture2D* BackIconTexture;

	UPROPERTY(EditAnywhere)
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere)
	EArmorType ArmorType;

protected:
	UPROPERTY()
	class UItemData* EquippedItemData;

// Variables to Bind UMG Components
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_BackIcon;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Equip;

protected:
	UFUNCTION()
	void OnWeaponUpdate(class UItemData* UpdateItemData, const bool& bEquip);

	UFUNCTION()
	void OnArmorUpdate(const EArmorType& UpdateArmorType, UItemData* UpdateItemData, const bool& bEquip);

public:
	UFUNCTION(BlueprintCallable)
	void SetSlot(class UItemData* UpdateItemData);

	UFUNCTION(BlueprintCallable)
	void ClearSlot();
};
