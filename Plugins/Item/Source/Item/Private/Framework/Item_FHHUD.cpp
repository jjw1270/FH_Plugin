// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Item_FHHUD.h"
#include "Item_HUDWidget.h"

void AItem_FHHUD::BeginPlay()
{
	Super::BeginPlay();

	// Create HUDWidget
	check(HUDWidgetClass);
	HUDWidget = CreateWidget<UItem_HUDWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->AddToViewport();
}

UItem_HUDWidget* AItem_FHHUD::GetHUDWidget()
{
	if (!IsValid(HUDWidget))
	{
		return nullptr;
	}

	return HUDWidget;
}
