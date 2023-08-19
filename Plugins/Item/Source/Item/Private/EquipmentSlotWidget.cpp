// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotWidget.h"
#include "Components/Image.h"

void UEquipmentSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Image_BackIcon->SetBrushFromTexture(BackIconTexture);
}
