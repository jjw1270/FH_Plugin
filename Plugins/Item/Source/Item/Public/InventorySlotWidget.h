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
	void SetItemDataToSlot(const struct FInventoryItem& InventoryItem);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image;

	UPROPERTY(BlueprintReadOnly)
	int32 Amount;

	UPROPERTY()
	class UItem_FHGameInstance* GameInstance;
	
protected:

};
