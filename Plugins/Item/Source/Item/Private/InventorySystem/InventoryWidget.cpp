// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Item.h"
#include "Blueprint/WidgetLayoutLibrary.h"
// Widget Components
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "InventorySlotWidget.h"

#include "InventoryComponent.h"
#include "Item_FHPlayerController.h"

#include "ItemData.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CreateSlotWidgets(DefaultSlotGridRowRange);

	// Bind UI Drag Event
	UIDragBtn->OnPressed.AddDynamic(this, &UInventoryWidget::OnDragBtnPressed);
	UIDragBtn->OnReleased.AddDynamic(this, &UInventoryWidget::OnDragBtnReleased);

	// Bind InventoryComponent Delegates
	BindInventoryCompEvents();
}

void UInventoryWidget::CreateSlotWidgets(int32 Row)
{
	CHECK_VALID(InventorySlotClass);

	for (int32 row = 0; row < Row; row++)
	{
		for (int32 col = 0; col < SlotGridColRange; col++)
		{
			UInventorySlotWidget* NewInventorySlot = Cast<UInventorySlotWidget>(CreateWidget(GetOwningPlayer(), InventorySlotClass));
			NewInventorySlot->SetOwningInventoryWidget(this);

			InventorySlotGrid->AddChildToUniformGrid(NewInventorySlot, row, col);
			InventorySlotArray.Add(NewInventorySlot);
		}
	}
}

void UInventoryWidget::BindInventoryCompEvents()
{
	AItem_FHPlayerController* PC = GetOwningPlayer<AItem_FHPlayerController>();

	if (IsValid(PC))
	{
		InventoryComp = PC->GetInventoryComp();
		if (IsValid(InventoryComp))
		{
			InventoryComp->ItemUpdateDelegate.AddUObject(this, &UInventoryWidget::OnItemUpdated);
			InventoryComp->ItemRegisterDelegate.AddUObject(this, &UInventoryWidget::OnItemRegister);

			return;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, this, &UInventoryWidget::BindInventoryCompEvents, 0.1f, false);
}

void UInventoryWidget::OnItemUpdated(class UItemData* UpdateItemData, const int32& UpdateAmount)
{
	// If Item is NonStackable, Add Item to New Slot
	if (UpdateItemData->GetUniqueID() > 1000)
	{
		AddNewItemToSlot(UpdateItemData, UpdateAmount);

		return;
	}

	// Check Item already exist in slot
	for (auto& InventorySlot : InventorySlotArray)
	{
		UItemData* InventorySlotItemData = InventorySlot->GetSlotItemData();
		if (!InventorySlotItemData)
		{
			continue;
		}

		if (InventorySlotItemData == UpdateItemData)
		{
			InventorySlot->SetSlot(UpdateItemData, UpdateAmount);

			return;
		}
	}

	// else Add Item to New Slot
	AddNewItemToSlot(UpdateItemData, UpdateAmount);
}

void UInventoryWidget::AddNewItemToSlot(class UItemData* NewItemData, const int32& NewItemAmount)
{
	for (auto& slot : InventorySlotArray)
	{
		if (slot->IsEmpty())
		{
			slot->SetSlot(NewItemData, NewItemAmount);
			return;
		}
	}

	//If All Slots are Full, Make New Slots On New Row
	CreateSlotWidgets(1);
	AddNewItemToSlot(NewItemData, NewItemAmount);
}

void UInventoryWidget::OnItemRegister(class UItemData* UpdateItemData, const bool& bIsRegist)
{
	// Set Register Image visibility
	for (auto& slot : InventorySlotArray)
	{
		UItemData* SlotItemData = slot->GetSlotItemData();
		if (!SlotItemData)
		{
			continue;
		}

		if (SlotItemData == UpdateItemData)
		{
			slot->SetOnRegistImageVisibility(bIsRegist);
			return;
		}
	}
}

// Sort according to IsEmpty()
void UInventoryWidget::SortItemSlot()
{
	for (int i = 0; i < InventorySlotArray.Num(); i++)
	{
		if (!InventorySlotArray[i]->IsEmpty())
		{
			continue;
		}

		for (int j = InventorySlotArray.Num()-1; j > i; j--)
		{
			if (!InventorySlotArray[j]->IsEmpty())
			{
				InventorySlotArray[i]->SetSlot(InventorySlotArray[j]->GetSlotItemData(), InventorySlotArray[j]->GetSlotItemAmount());
				InventorySlotArray[j]->ClearSlot();

				break;
			}
		}
	}
}

void UInventoryWidget::OnDragBtnPressed()
{
	MousePosOnDragStart = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld())
		- Cast<UCanvasPanelSlot>(InventoryUI->Slot)->GetPosition();

	GetWorld()->GetTimerManager().SetTimer(DragTimerHandle, this, &UInventoryWidget::DragUI, 0.01f, true);
}

void UInventoryWidget::DragUI()
{
	FVector2D UIPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()) - MousePosOnDragStart;

	Cast<UCanvasPanelSlot>(InventoryUI->Slot)->SetPosition(UIPos);
}

void UInventoryWidget::OnDragBtnReleased()
{
	if (DragTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DragTimerHandle);
	}
}
