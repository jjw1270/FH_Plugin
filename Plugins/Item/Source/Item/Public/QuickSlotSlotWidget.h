// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickSlotSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UQuickSlotSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
// Native Func
protected:
	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void SetSlot(const int32& NewItemID);

	void OnUpdateItem(const int32& UpdateItemID);

	void SetWidgetBindVariables();

	void ClearSlot();

protected:
	UPROPERTY(BlueprintReadOnly)
	class UQuickSlotWidget* QuickSlotWidget;

	UPROPERTY(BlueprintReadOnly)
	class UInventoryComponent* InventoryComp;

protected:
	int32 ItemID;

	FDelegateHandle OnInventoryItemChangedHandle;

// bind widget vars
public:
	UPROPERTY(BlueprintReadOnly)
	int32 QuickSlotSlotNum;

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImageWidget;

	UPROPERTY(BlueprintReadOnly)
	int32 Amount;

public:
	FORCEINLINE int32 GetSlotItemID() const { return ItemID; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsEmpty() const { return Amount <= 0; }
};
