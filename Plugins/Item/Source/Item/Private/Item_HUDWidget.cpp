// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_HUDWidget.h"

void UItem_HUDWidget::OpenWidget(UUserWidget* Widget)
{
	Widget->SetVisibility(ESlateVisibility::Visible);
}

void UItem_HUDWidget::CloseWidget(UUserWidget* Widget)
{
	Widget->SetVisibility(ESlateVisibility::Collapsed);
}
