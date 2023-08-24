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
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UOnDragWidget> DragWidgetClass;

public:
	void SetOwningInventoryWidget(class UInventoryWidget* NewInventoryWidget);

	FORCEINLINE UInventoryWidget* GetOwningInventoryWidget() const { return InventoryWidget; }

public:
	UFUNCTION(BlueprintCallable)
	void SetSlot(class UItemData* NewItemData, const int32& NewAmount);

	UFUNCTION(BlueprintCallable)
	void ClearSlot();

	UFUNCTION(BlueprintCallable)
	void SetOnRegistImageVisibility(const bool& bIsRegist);

protected:
	UPROPERTY(BlueprintReadOnly)
	class UInventoryComponent* InventoryComp;

	UPROPERTY(BlueprintReadOnly)
	class UInventoryWidget* InventoryWidget;

// Variables to Bind UMG Components
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Item;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_OnRegist;
	
protected:
	UPROPERTY()
	class UItemData* SlotItemData;

	UPROPERTY()
	int32 SlotItemAmount;

public:	
	UFUNCTION(BlueprintCallable)
	bool IsEmpty();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UItemData* GetSlotItemData() const { return SlotItemData; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetSlotItemAmount() const { return SlotItemAmount; }

};
