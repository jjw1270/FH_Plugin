// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY()
	class UInventoryComponent* InventoryComp;

	FTimerHandle InitTimerHandle;

// Blueprint Bind Widgets
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInventorySlotWidget> InventorySlotClass;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UInventorySlotWidget*> InventorySlotArray;

protected:
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* InventoryUI;

	UPROPERTY(meta = (BindWidget))
	class UButton* UIDragBtn;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* InventorySlotGrid;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* ItemInfoBox;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* ItemTrash;

	UPROPERTY(meta = (BindWidget))
	class URemoveConfirmWidget* RemoveConfirmWidget;
	
protected:
	UPROPERTY(EditAnywhere)
	int32 DefaultSlotGridRowRange;

	int32 SlotGridColRange{ 5 };

	FVector2D MousePosOnDragStart;

	FTimerHandle DragTimerHandle;

public:
	void AddNewItemToSlot(class UItemData* NewItemData, const int32& NewItemAmount);

protected:
	void CreateSlotWidgets(int32 Row);

	void BindInventoryCompEvents();

	UFUNCTION()
	void OnItemUpdated(class UItemData* UpdateItemData, const int32& UpdateAmount);

	UFUNCTION()
	void OnItemRegister(class UItemData* UpdateItemData, const bool& bIsRegist);

	UFUNCTION(BlueprintCallable)
	void SortItemSlot();

//Drag Func
	UFUNCTION()
	void OnDragBtnPressed();

	void DragUI();

	UFUNCTION()
	void OnDragBtnReleased();

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UUserWidget* GetItemInfoBox() const { return ItemInfoBox; }

	FORCEINLINE UUserWidget* GetItemTrash() const { return ItemTrash; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class URemoveConfirmWidget* GetRemoveConfirmWidget() const { return RemoveConfirmWidget; }

};
