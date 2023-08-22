// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"

void UEquipmentWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UIDragBtn->OnPressed.AddDynamic(this, &UEquipmentWidget::OnDragBtnPressed);
	UIDragBtn->OnReleased.AddDynamic(this, &UEquipmentWidget::OnDragBtnReleased);
}

void UEquipmentWidget::OnDragBtnPressed()
{
	MousePosOnDragStart = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld())
		- Cast<UCanvasPanelSlot>(EquipmentUI->Slot)->GetPosition();

	GetWorld()->GetTimerManager().SetTimer(DragTimerHandle, this, &UEquipmentWidget::DragUI, 0.01f, true);
}

void UEquipmentWidget::DragUI()
{
	FVector2D UIPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()) - MousePosOnDragStart;

	Cast<UCanvasPanelSlot>(EquipmentUI->Slot)->SetPosition(UIPos);
}

void UEquipmentWidget::OnDragBtnReleased()
{
	if (DragTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DragTimerHandle);
	}
}
