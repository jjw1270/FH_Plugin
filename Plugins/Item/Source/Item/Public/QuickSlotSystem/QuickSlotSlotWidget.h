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

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetSlot(class UItemData* NewItemData, const int32& NewItemAmount);

	UFUNCTION(BlueprintCallable)
	void ClearSlot();

protected:
	UFUNCTION()
	void OnUpdateItem(class UItemData* UpdateItemData, const int32& UpdateAmount);

protected:
	UPROPERTY(BlueprintReadOnly)
	class UInventoryComponent* InventoryComp;

	UPROPERTY(BlueprintReadOnly)
	class UQuickSlotComponent* QuickSlotComp;

	UPROPERTY(BlueprintReadOnly)
	class UQuickSlotWidget* QuickSlotWidget;

	FDelegateHandle ItemUpdateDelegateHandle;

// Variables to Bind UMG Components
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImageWidget;

protected:
	UPROPERTY(BlueprintReadOnly)
	int32 Index;

	UPROPERTY(BlueprintReadOnly)
	class UItemData* SlotItemData;

	UPROPERTY(BlueprintReadOnly)
	int32 SlotItemAmount;

public:
	FORCEINLINE void SetIndex(const int32& NewIndex) { Index = NewIndex; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsEmpty() const { return !IsValid(SlotItemData); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UItemData* GetSlotItemData() const { return SlotItemData; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetSlotItemAmount() const { return SlotItemAmount; }

};
