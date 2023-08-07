// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "InventorySlotWidget.h"

#include "ItemInterface.h"
#include "InventoryComponent.h"
#include "Item_FHPlayerController.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(LogTemp, Warning, TEXT("Initwidget"));

	PC = Cast<AItem_FHPlayerController>(GetOwningPlayer());
	ensureMsgf(PC, TEXT("PC is nullptr"));

	InventoryComp = PC->GetInventoryComp();
	ensureMsgf(InventoryComp, TEXT("InventoryComp is nullptr"));

	ensureMsgf(InventorySlotClass, TEXT("InventorySlotClass is nullptr"));
	for (int32 row = 0; row < SlotGridRowRange; row++)
	{
		for (int32 col = 0; col < SlotGridColRange; col++)
		{
			UInventorySlotWidget* NewInventorySlot = Cast<UInventorySlotWidget>(CreateWidget(GetWorld(), InventorySlotClass));
			InventorySlotGrid->AddChildToUniformGrid(NewInventorySlot, row, col);
			InventorySlotArray.Add(NewInventorySlot);
		}
	}
}

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UInventoryWidget::NativeDestruct()
{

}

void UInventoryWidget::AddItemToSlot(FInventoryItem* NewItem)
{
	for (auto slot : InventorySlotArray)
	{
		if (slot->IsEmpty())
		{
			slot->SetItemDataToSlot(NewItem, InventoryComp);
			return;
		}
	}
}

void UInventoryWidget::UpdateItemToSlot(FInventoryItem* NewItem)
{
	for (auto slot : InventorySlotArray)
	{
		if (slot->GetSlotInventoryItem()->ID == NewItem->ID)
		{
			UE_LOG(LogTemp, Warning, TEXT("AddItemAmount"));
			slot->AddItemAmount(NewItem->Amount);
			return;
		}
	}
}
