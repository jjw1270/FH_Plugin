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
	
protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

public:
	void SetItemDataToSlot(struct FInventoryItem* InventoryItem);

	void UpdateItemAmount();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> DragWidgetClass;

	UPROPERTY()
	UUserWidget* DragWidget;

	UPROPERTY()
	TSubclassOf<class UItemDragDropOperation> DragOperationClass;

	UPROPERTY()
	class UItemDragDropOperation* DragOperation;

protected:
	UPROPERTY(BlueprintReadOnly)
	class UInventoryComponent* InventoryComp;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* ItemInfoBox;

protected:
	FInventoryItem* SlotInventoryItem;

protected:
	UFUNCTION(BlueprintCallable)
	void SetWidgetBindVariables(bool bReset);

protected:
	UPROPERTY(BlueprintReadOnly)
	int32 Amount;

	UPROPERTY(BlueprintReadOnly)
	EItemType ItemType;

	UPROPERTY(BlueprintReadOnly)
	FString ItemName;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemPrice;

	UPROPERTY(BlueprintReadOnly)
	FString ItemInfo;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

public:	
	FORCEINLINE FInventoryItem* GetSlotInventoryItem() const { return SlotInventoryItem; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsEmpty() const { return Amount <= 0; }

};
