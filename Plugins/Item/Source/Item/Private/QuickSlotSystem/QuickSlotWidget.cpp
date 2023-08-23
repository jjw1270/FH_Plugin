// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotWidget.h"
#include "Item.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Item_FHPlayerController.h"
#include "QuickSlotComponent.h"
#include "QuickSlotSlotWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/Border.h"

void UQuickSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Create QuickSlot Slots
	CHECK_VALID(QuickSlotSlotClass);
	for (int32 col = 0; col < 6; col++)
	{
		UQuickSlotSlotWidget* NewQuickSlotSlot = Cast<UQuickSlotSlotWidget>(CreateWidget(GetOwningPlayer(), QuickSlotSlotClass));
		NewQuickSlotSlot->SetIndex(col);

		QuickSlotGrid->AddChildToUniformGrid(NewQuickSlotSlot, 0, col);
		QuickSlotSlotArray.Add(NewQuickSlotSlot);
	}

	// Bind UI Drag Event
	UIDragBtn->OnPressed.AddDynamic(this, &UQuickSlotWidget::OnDragBtnPressed);
	UIDragBtn->OnReleased.AddDynamic(this, &UQuickSlotWidget::OnDragBtnReleased);

	// Bind QuickSlotComponent Delegates
	BindQuickSlotCompEvents();
}

void UQuickSlotWidget::BindQuickSlotCompEvents()
{
	AItem_FHPlayerController* PC = GetOwningPlayer<AItem_FHPlayerController>();

	if (IsValid(PC))
	{
		QuickSlotComp = PC->GetQuickSlotComp();
		if (IsValid(QuickSlotComp))
		{
			QuickSlotComp->QuickSlotUpdateDelegate.AddUObject(this, &UQuickSlotWidget::OnQuickSlotUpdated);

			return;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, this, &UQuickSlotWidget::BindQuickSlotCompEvents, 0.1f, false);
}

void UQuickSlotWidget::OnQuickSlotUpdated(const int32& QuickSlotIndex, UItemData* NewItemData, const int32& NewItemAmount)
{
	UQuickSlotSlotWidget* QuickSlotSlot = QuickSlotSlotArray[QuickSlotIndex];

	QuickSlotSlot->SetSlot(NewItemData, NewItemAmount);
}

void UQuickSlotWidget::OnDragBtnPressed()
{
	MousePosOnDragStart = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld())
		- Cast<UCanvasPanelSlot>(QuickSlotUI->Slot)->GetPosition();

	GetWorld()->GetTimerManager().SetTimer(DragTimerHandle, this, &UQuickSlotWidget::DragUI, 0.01f, true);
}

void UQuickSlotWidget::DragUI()
{
	FVector2D UIPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()) - MousePosOnDragStart;

	Cast<UCanvasPanelSlot>(QuickSlotUI->Slot)->SetPosition(UIPos);
}

void UQuickSlotWidget::OnDragBtnReleased()
{
	if (DragTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DragTimerHandle);
	}
}
