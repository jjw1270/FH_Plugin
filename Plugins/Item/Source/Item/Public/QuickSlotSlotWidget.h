// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlotWidget.h"
#include "QuickSlotSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UQuickSlotSlotWidget : public UInventorySlotWidget
{
	GENERATED_BODY()
	
// Native Func
protected:
	void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	virtual void SetWidgetBindVariables() override;

	virtual void ClearSlot() override;

public:
	UPROPERTY(BlueprintReadOnly)
	int32 QuickSlotSlotNum;
};
