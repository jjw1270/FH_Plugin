// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentWidget.h"
#include "Item.h"
#include "Components/Button.h"
#include "Item_FHHUD.h"
#include "Item_HUDWidget.h"

void UEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UIDragBtn->OnPressed.AddDynamic(this, &UEquipmentWidget::OnDragBtnPressed);
	UIDragBtn->OnReleased.AddDynamic(this, &UEquipmentWidget::OnDragBtnReleased);
}

void UEquipmentWidget::OnDragBtnPressed()
{
	if (!HUDWidget)
	{
		AItem_FHHUD* HUD = Cast<AItem_FHHUD>(GetOwningPlayer()->GetHUD());
		CHECK_VALID(HUD);

		HUDWidget = HUD->GetHUDWidget();
	}

	HUDWidget->WidgetDragStart(this);
}

void UEquipmentWidget::OnDragBtnReleased()
{
	CHECK_VALID(HUDWidget);

	HUDWidget->WidgetDragEnd();
}
