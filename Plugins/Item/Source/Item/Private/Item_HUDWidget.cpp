// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_HUDWidget.h"
#include "Item.h"
#include "GameFramework/PlayerController.h"

#include "QuickSlotWidget.h"
#include "InventoryWidget.h"
#include "EquipmentWidget.h"

void UItem_HUDWidget::SwichWidgetVisibility(class APlayerController* TargetPlayerController, const FName& WidgetName)
{
	UUserWidget* Widget = GetUserwidgetFromFName(WidgetName);
	if (Widget == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("AASFD"));
		return;
	}
	CHECK_VALID(Widget);

	switch (Widget->GetVisibility())
	{
	//Open widget
	case ESlateVisibility::Collapsed:
		Widget->SetVisibility(ESlateVisibility::Visible);
		TargetPlayerController->SetShowMouseCursor(true);
		TargetPlayerController->SetInputMode(FInputModeGameAndUI());
		break;
	//Close widget
	case ESlateVisibility::Visible:
		Widget->SetVisibility(ESlateVisibility::Collapsed);
		TargetPlayerController->SetShowMouseCursor(false);
		TargetPlayerController->SetInputMode(FInputModeGameOnly());
		break;
	default:
		break;
	}
}

UUserWidget* UItem_HUDWidget::GetUserwidgetFromFName(const FName& WidgetName)
{
	if (WidgetName == QuickSlotWidget->GetFName())
	{
		return QuickSlotWidget;
	}
	if(WidgetName == InventoryWidget->GetFName())
	{
		return InventoryWidget;
	}
	if (WidgetName == EquipmentWidget->GetFName())
	{
		return EquipmentWidget;
	}

	return nullptr;
}
