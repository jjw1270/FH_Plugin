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

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UOnDragWidget> DragWidgetClass;

public:
	void SetOwningInventoryWidget(class UInventoryWidget* NewInventoryWidget);

	FORCEINLINE UInventoryWidget* GetOwningInventoryWidget() const { return InventoryWidget; }

public:
	void SetSlot(const int32& NewItemID);

	void OnUpdateItem(const int32& UpdateItemID);

	UFUNCTION(BlueprintCallable)
	virtual void SetWidgetBindVariables();

	virtual void ClearSlot();

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
	int32 Amount;

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImageWidget;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EItemType ItemType;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 ItemPrice;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString ItemInfo;

public:	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsEmpty() const { return Amount <= 0; }

	FORCEINLINE int32 GetSlotItemID() const { return ItemID; }

	UTexture2D* GetItemImage();
};
