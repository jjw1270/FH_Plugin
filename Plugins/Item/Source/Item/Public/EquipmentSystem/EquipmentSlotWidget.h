// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemType.h"
#include "EquipmentSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	void InitSlot();

protected:
	FTimerHandle th_InitSlot;

	UPROPERTY()
	class UInventoryComponent* InventoryComp;

	UPROPERTY()
	class AItem_FHPlayerController* PC;

	UPROPERTY()
	class UEquipmentComponent* EquipComp;

protected:
	UPROPERTY(EditAnywhere)
	EEquipmentType EquipmentType;

	UPROPERTY(EditAnywhere)
	UTexture2D* BackIconTexture;

	UPROPERTY(BlueprintReadOnly)
	EItemType ItemType = EItemType::Equipment;

	UPROPERTY(BlueprintReadOnly)
	FString ItemName;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemPrice;

	UPROPERTY(BlueprintReadOnly)
	FString ItemInfo;
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_BackIcon;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Equip;

protected:
	void OnEquipmentChanged(const EEquipmentType& EquipType, const int32& ItemID, const bool& bEquip);

	void SetWidgetBindVariables(FEquipmentItemData* ItemData);

	void ClearSlot(const int32& ItemID);
};
