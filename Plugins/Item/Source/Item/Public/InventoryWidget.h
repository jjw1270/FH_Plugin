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
	class UItem_FHGameInstance* GameInstance;

	UPROPERTY(BlueprintReadOnly)
	class AItem_FHPlayerController* PlayerController;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* InventorySlotGrid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UInventorySlotWidget> InventorySlotClass;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UInventorySlotWidget*> InventorySlotArray;
	
	UPROPERTY(EditAnywhere)
	int32 SlotGridRowRange;

	int32 SlotGridColRange{ 5 };

	FTimerHandle UpdateInventoryHandle;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AddSlotGridRowRange(int32 Num = 1) { SlotGridRowRange  += Num; }

	void SetItemsToSlots();

};
