// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item_HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UItem_HUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SwichWidgetVisibility(class APlayerController* TargetPlayerController, const FName& WidgetName);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UQuickSlotWidget* QuickSlotWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UInventoryWidget* InventoryWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UEquipmentWidget* EquipmentWidget;

protected:
	UUserWidget* GetUserwidgetFromFName(const FName& WidgetName);

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UQuickSlotWidget* GetQuickSlotWidget() const { return QuickSlotWidget; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UEquipmentWidget* GetEquipmentWidget() const { return EquipmentWidget; }

};
