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

public:
	void AddItemAmount(int32 NewAmount);

	void SetItemDataToSlot(struct FInventoryItem* InventoryItem, class UInventoryComponent* InventoryComp);

protected:
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* ItemInfoBox;

	FInventoryItem* SlotInventoryItem;

	UPROPERTY(BlueprintReadOnly)
	FString ItemType;

	UPROPERTY(BlueprintReadOnly)
	FString ItemName;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemPrice;

	UPROPERTY(BlueprintReadOnly)
	FString ItemInfo;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;

	UPROPERTY(BlueprintReadOnly)
	int32 Amount;

public:	
	FORCEINLINE FInventoryItem* GetSlotInventoryItem() const { return SlotInventoryItem; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsEmpty() const { return Amount <= 0; }

};
