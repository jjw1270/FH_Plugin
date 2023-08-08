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
	UPROPERTY(BlueprintReadOnly)
	class AItem_FHPlayerController* PC;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* InventorySlotGrid;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* ItemInfoBox;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* ItemTrash;

	UPROPERTY(meta = (BindWidget))
	URemoveConfirmWidget* RemoveConfirmWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInventorySlotWidget> InventorySlotClass;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UInventorySlotWidget*> InventorySlotArray;
	
	UPROPERTY(EditAnywhere)
	int32 SlotGridRowRange;

	int32 SlotGridColRange{ 5 };

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddSlotGridRowRange(int32 Num = 1) { SlotGridRowRange  += Num; }

	FORCEINLINE UUserWidget* GetItemInfoBox() const { return ItemInfoBox; }

	FORCEINLINE UUserWidget* GetItemTrash() const { return ItemTrash; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class URemoveConfirmWidget* GetRemoveConfirmWidget() const { return RemoveConfirmWidget; }

	void AddItemToSlot(FInventoryItem* NewItem);

	void UpdateItemToSlot(FInventoryItem* NewItem);

};
