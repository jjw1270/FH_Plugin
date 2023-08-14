// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
// Native Func
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	void SetOwningInventoryWidget(class UInventoryWidget* NewInventoryWidget);

public:
	void SetSlot(const int32& NewItemID);

	void OnUpdateItem(const int32& UpdateItemID);

	UFUNCTION(BlueprintCallable)
	void SetWidgetBindVariables();

	void ClearSlot();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UOnDragWidget> DragWidgetClass;

protected:
	UPROPERTY(BlueprintReadOnly)
	class UInventoryComponent* InventoryComp;

	UPROPERTY(BlueprintReadOnly)
	class UInventoryWidget* InventoryWidget;

// FInventoryItem In this Slot 
protected:
	int32 ItemID;

	FDelegateHandle OnInventoryItemChangedHandle;

// Variables to Bind UMG Components
protected:
	UPROPERTY(BlueprintReadOnly)
	EItemType ItemType;

	UPROPERTY(BlueprintReadOnly)
	FString ItemName;

	UPROPERTY(BlueprintReadOnly)
	int32 Amount;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemPrice;

	UPROPERTY(BlueprintReadOnly)
	FString ItemInfo;

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImageWidget;

public:	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsEmpty() const { return Amount <= 0; }

	FORCEINLINE int32 GetSlotItemID() const { return ItemID; }

	UTexture2D* GetItemImage();
};
