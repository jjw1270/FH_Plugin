// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoveConfirmWidget.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "ItemDragDropOperation.h"
#include "Item_FHPlayerController.h"
#include "InventoryComponent.h"
#include "InventorySlotWidget.h"

void URemoveConfirmWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(GetOwningPlayer());
	ensureMsgf(PC, TEXT("PC is nullptr"));

	InventoryComp = PC->GetInventoryComp();
	ensureMsgf(InventoryComp, TEXT("InventoryComp is nullptr"));
}

void URemoveConfirmWidget::ShowRemoveConfirm(UItemDragDropOperation* NewItemDragDropOperation)
{
	ItemDragDropOperation = NewItemDragDropOperation;

	ItemImage->SetBrushFromTexture(ItemDragDropOperation->ItemImage);
	MaxAmount = ItemDragDropOperation->SlotInventoryItem->Amount;
	Slider_Amount->SetValue(1.f);

	FString AmountTxt = FString::FromInt(static_cast<int32>(Slider_Amount->GetValue()));
	Text_Amount->SetText(FText::FromString(AmountTxt));

	if (MaxAmount <= 1)
	{
		SetAmountBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Slider_Amount->SetMaxValue(MaxAmount);
	}

	GetParent()->SetVisibility(ESlateVisibility::Visible);
}

void URemoveConfirmWidget::RemoveItem()
{
	int32 Amount = Slider_Amount->GetValue();

	if (ItemDragDropOperation)
	{
		int32 AmountLeft = InventoryComp->RemoveItemFromInventory(ItemDragDropOperation->SlotInventoryItem, Amount);

		if (AmountLeft > 0)
		{
			UInventorySlotWidget* PrevInventorySlot = Cast<UInventorySlotWidget>(ItemDragDropOperation->Payload);
			if (PrevInventorySlot)
			{
				PrevInventorySlot->SetItemDataToSlot(ItemDragDropOperation->SlotInventoryItem);
			}
		}
	}

	GetParent()->SetVisibility(ESlateVisibility::Collapsed);
}

void URemoveConfirmWidget::OnCancel()
{
	UInventorySlotWidget* PrevInventorySlot = Cast<UInventorySlotWidget>(ItemDragDropOperation->Payload);
	if (PrevInventorySlot)
	{
		PrevInventorySlot->SetItemDataToSlot(ItemDragDropOperation->SlotInventoryItem);
	}

	SetAmountBox->SetVisibility(ESlateVisibility::Visible);

	GetParent()->SetVisibility(ESlateVisibility::Collapsed);
}
