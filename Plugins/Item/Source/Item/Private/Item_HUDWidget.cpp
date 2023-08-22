// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_HUDWidget.h"
#include "Item.h"
#include "Item_FHPlayerController.h"

void UItem_HUDWidget::SwichWidgetVisibility(const FName& WidgetName)
{
	AItem_FHPlayerController* PC = GetOwningPlayer<AItem_FHPlayerController>();
	CHECK_VALID(PC);

	UUserWidget* Widget = GetUserwidgetFromFName(WidgetName);
	CHECK_VALID(Widget);

	switch (Widget->GetVisibility())
	{
	//Open widget
	case ESlateVisibility::Collapsed:
		Widget->SetVisibility(ESlateVisibility::Visible);
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeGameAndUI());
		break;
	//Close widget
	case ESlateVisibility::Visible:
		Widget->SetVisibility(ESlateVisibility::Collapsed);
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
		break;
	default:
		break;
	}
}

UUserWidget* UItem_HUDWidget::GetUserwidgetFromFName(const FName& WidgetName)
{
	if (WidgetName == Cast<UUserWidget>(QuickSlotWidget)->GetFName())
	{
		return Cast<UUserWidget>(QuickSlotWidget);
	}
	else if(WidgetName == Cast<UUserWidget>(InventoryWidget)->GetFName())
	{
		return Cast<UUserWidget>(InventoryWidget);
	}
	else if (WidgetName == Cast<UUserWidget>(EquipmentWidget)->GetFName())
	{
		return Cast<UUserWidget>(EquipmentWidget);
	}
	else
	{
		return nullptr;
	}
}
