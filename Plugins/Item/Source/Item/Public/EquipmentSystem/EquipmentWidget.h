// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UEquipmentWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	// Blueprint Bind Widgets
protected:
	UPROPERTY(meta = (BindWidget))
	class UBorder* EquipmentUI;

	UPROPERTY(meta = (BindWidget))
	class UButton* UIDragBtn;

	FVector2D MousePosOnDragStart;

	FTimerHandle DragTimerHandle;

protected:
	UFUNCTION()
	void OnDragBtnPressed();

	void DragUI();

	UFUNCTION()
	void OnDragBtnReleased();
};
