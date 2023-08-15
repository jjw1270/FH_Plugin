// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotWidget.h"
#include "QuickSlotSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"

void UQuickSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(QuickSlotSlotClass);
	for (int32 col = 0; col < 6; col++)
	{
		UQuickSlotSlotWidget* NewQuickSlotSlot = Cast<UQuickSlotSlotWidget>(CreateWidget(GetOwningPlayer(), QuickSlotSlotClass));
		NewQuickSlotSlot->QuickSlotSlotNum = col + 1;
		QuickSlotGrid->AddChildToUniformGrid(NewQuickSlotSlot, 0, col);
		QuickSlotSlots.Add(NewQuickSlotSlot);
	}
}

UQuickSlotSlotWidget* UQuickSlotWidget::GetQuickSlotSlot(int32 SlotNum)
{
	return QuickSlotSlots[SlotNum-1];
}
