// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY()
	TArray<class UQuickSlotSlotWidget*> QuickSlotSlots;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* QuickSlotGrid;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UQuickSlotSlotWidget> QuickSlotSlotClass;

public:
	TArray<class UQuickSlotSlotWidget*>* GetQuickSlotSlotArray();

	class UQuickSlotSlotWidget* GetQuickSlotSlot(int32 SlotNum);
};
