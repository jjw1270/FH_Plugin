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

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

protected:
	bool bIsCreateSlots;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* InventorySlotGrid;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInventorySlotWidget> InventorySlotClass;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UInventorySlotWidget*> InventorySlotArray;
	
	UPROPERTY(EditAnywhere)
	int32 SlotGridRowRange;

	int32 SlotGridColRange{ 5 };


	UPROPERTY()
	class UInventoryComponent* InventoryComp;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddSlotGridRowRange(int32 Num = 1) { SlotGridRowRange  += Num; }

	void SetItemsToSlots(TArray<FInventoryItem>& InventoryItemArray);

};
