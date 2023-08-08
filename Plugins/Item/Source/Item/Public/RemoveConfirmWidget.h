// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RemoveConfirmWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API URemoveConfirmWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
protected:
	UPROPERTY()
	class UInventoryComponent* InventoryComp;

	UPROPERTY()
	class UItemDragDropOperation* ItemDragDropOperation;

protected:
	UPROPERTY(BlueprintReadOnly)
	int32 MaxAmount;

	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

public:
	UFUNCTION(BlueprintCallable)
	void ShowRemoveConfirm(class UItemDragDropOperation* NewItemDragDropOperation);

protected:
	UFUNCTION(BlueprintCallable)
	void RemoveItem(int32 Amount);

	UFUNCTION(BlueprintCallable)
	void OnCancel();
};
