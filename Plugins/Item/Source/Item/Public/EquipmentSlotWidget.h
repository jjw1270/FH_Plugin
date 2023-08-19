// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	UPROPERTY(EditAnywhere)
	UTexture2D* BackIconTexture;
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_BackIcon;

};
