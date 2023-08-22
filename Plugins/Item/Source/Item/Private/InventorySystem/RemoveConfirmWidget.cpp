// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoveConfirmWidget.h"
#include "Item.h"
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

	AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(GetOwningPlayer());
	CHECK_VALID(PC);

	InventoryComp = PC->GetInventoryComp();
	CHECK_VALID(InventoryComp);

	ClearWidget();
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

	InventoryComp->RemoveItemFromInventory(ItemData, Amount);

	SetAmountBox->SetVisibility(ESlateVisibility::Visible);
	GetParent()->SetVisibility(ESlateVisibility::Collapsed);

	ClearWidget();
}

void URemoveConfirmWidget::OnCancel()
{
	SetAmountBox->SetVisibility(ESlateVisibility::Visible);
	GetParent()->SetVisibility(ESlateVisibility::Collapsed);

	ClearWidget();
}

void URemoveConfirmWidget::ClearWidget()
{
	ItemData = nullptr;
	MaxAmount = 0;

	Slider_Amount->SetValue(1.f);

	Text_Amount->SetText(FText::FromString(TEXT("1")));
}
