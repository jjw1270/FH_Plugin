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
	void OpenWidget(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void CloseWidget(UUserWidget* Widget);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UQuickSlotWidget* QuickSlotWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UInventoryWidget* InventoryWidget;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UQuickSlotWidget* GetQuickSlotWidget() const { return QuickSlotWidget; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

};
