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

	PC = Cast<AItem_FHPlayerController>(GetOwningPlayer());
	ensureMsgf(PC, TEXT("PC is nullptr"));

	InventoryComp = PC->GetInventoryComp();
	ensureMsgf(InventoryComp, TEXT("InventoryComp is nullptr"));
}

void URemoveConfirmWidget::ShowRemoveConfirm(UItemDragDropOperation* NewDragDropOperation)
{
	ItemID = NewDragDropOperation->DragingItemID;

	MaxAmount = *InventoryComp->GetInventoryItems()->Find(ItemID);
	
	EItemType ItemType = InventoryComp->GetItemType(ItemID);
	switch (ItemType)
	{
	case EItemType::Consumable:
	{
		FConsumableItemData* ItemData = InventoryComp->GetConsumableItemInfo(ItemID);
		ItemName = ItemData->Name;
		Image_Item->SetBrushFromTexture(ItemData->ItemImage);
		break;
	}
	case EItemType::Equipment:
	{
		FEquipmentItemData* ItemData = InventoryComp->GetEquipmentItemInfo(ItemID);
		ItemName = ItemData->Name;
		Image_Item->SetBrushFromTexture(ItemData->ItemImage);
		break;
	}
	default:
		break;
	}

	Slider_Amount->SetValue(1.f);

	if (MaxAmount > 1)
	{
		Text_Amount->SetText(FText::FromString(TEXT("1")));

		Slider_Amount->SetMaxValue(MaxAmount);
	}
	else
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
