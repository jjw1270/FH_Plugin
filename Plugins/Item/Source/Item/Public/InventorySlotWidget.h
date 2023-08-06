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
	bool IsEmpty();

	void SetItemDataToSlot(const struct FInventoryItem& InventoryItem);

protected:
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

};
