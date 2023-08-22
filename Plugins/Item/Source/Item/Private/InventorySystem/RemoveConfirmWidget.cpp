// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoveConfirmWidget.h"
#include "ItemData.h"
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

	PC = Cast<AItem_FHPlayerController>(GetOwningPlayer());
	ensureMsgf(PC, TEXT("PC is nullptr"));

	InventoryComp = PC->GetInventoryComp();
	ensureMsgf(InventoryComp, TEXT("InventoryComp is nullptr"));
}

void URemoveConfirmWidget::ShowRemoveConfirm(UItemDragDropOperation* NewDragDropOperation)
{
	ItemData = NewDragDropOperation->DraggingItemData;
	MaxAmount = NewDragDropOperation->DraggingItemAmount;

	Slider_Amount->SetMaxValue(MaxAmount);

	FBaseItemData BaseItemData;
	if (!ItemData->GetBaseData(BaseItemData))
	{
		return;
	}

	Image_Item->SetBrushFromTexture(BaseItemData.Icon2D);

	if (MaxAmount == 1)
	{
		SetAmountBox->SetVisibility(ESlateVisibility::Collapsed);
	}

	GetParent()->SetVisibility(ESlateVisibility::Visible);
}

void URemoveConfirmWidget::RemoveItem()
{
	int32 Amount = Slider_Amount->GetValue();

	InventoryComp->RemoveItemFromInventory(ItemID, Amount);

	SetAmountBox->SetVisibility(ESlateVisibility::Visible);
	GetParent()->SetVisibility(ESlateVisibility::Collapsed);
}

void URemoveConfirmWidget::OnCancel()
{
	ItemID = 0;

	SetAmountBox->SetVisibility(ESlateVisibility::Visible);
	GetParent()->SetVisibility(ESlateVisibility::Collapsed);
}

void URemoveConfirmWidget::ClearWidget()
{
	Slider_Amount->SetValue(1.f);

	Text_Amount->SetText(FText::FromString(TEXT("1")));
}
