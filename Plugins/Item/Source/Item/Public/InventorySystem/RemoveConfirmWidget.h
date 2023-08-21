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
	class AItem_FHPlayerController* PC;

	UPROPERTY()
	class UInventoryComponent* InventoryComp;

protected:
	int32 ItemID;

protected:
	UPROPERTY(BlueprintReadOnly)
	int32 MaxAmount;

	UPROPERTY(BlueprintReadOnly)
	FString ItemName;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Item;

	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_Amount;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* SetAmountBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Text_Amount;

public:
	UFUNCTION(BlueprintCallable)
	void ShowRemoveConfirm(class UItemDragDropOperation* NewDragDropOperation);

protected:
	UFUNCTION(BlueprintCallable)
	void RemoveItem();

	UFUNCTION(BlueprintCallable)
	void OnCancel();
};
