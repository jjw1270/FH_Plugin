// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotSystem/QuickSlotComponent.h"
#include "Item.h"
#include "ItemData.h"
#include "Item_FHGameInstance.h"
#include "Item_FHPlayerController.h"
#include "InventoryComponent.h"

UQuickSlotComponent::UQuickSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	InitComponent();
}

void UQuickSlotComponent::InitComponent()
{
	UItem_FHGameInstance* GI = Cast<UItem_FHGameInstance>(GetOwner()->GetGameInstance());
	CHECK_VALID(GI);

	AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(GetOwner());
	CHECK_VALID(PC);

	QuickSlotItems = GI->GetQuickSlotItems();

	InventoryComp = PC->GetInventoryComp();
	CHECK_VALID(InventoryComp);
}

void UQuickSlotComponent::ManageQuickSlot(UItemData* TargetItemData, const int32& TargetItemAmount)
{
	// Check QuickSlot is empty
	int32 QuickSlotIndex = GetEmptyQuickSlotSlotIndex();

	// If QuickSlot is Full, Do Nothing
	if (QuickSlotIndex < 0)
	{
		return;
	}

	// check item is already in quickslot
	// if true, Delete QuickSlot Item
	int32 ItemExistInQuickSlotIndex;
	if (IsItemExistInQuickSlot(TargetItemData, ItemExistInQuickSlotIndex))
	{
		DeleteItemFromQuickSlot(ItemExistInQuickSlotIndex);

		return;
	}

	// else Set Item to QuickSlot
	SetItemToQuickSlot(QuickSlotIndex, TargetItemData, TargetItemAmount);
}

void UQuickSlotComponent::UseQuickSlotItem(const int32& TargetQuickSlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("UseQuickSlotItem : %d"), TargetQuickSlotIndex)
	/////////////////////////////////////////////////////////////
}

void UQuickSlotComponent::SetItemToQuickSlot(const int32& NewQuickSlotIndex, class UItemData* NewItemData, const int32& NewItemAmount)
{
	// Check item already exist in quick slots
	// if true, delete exist item.
	int32 ItemExistInQuickSlotIndex;
	if (IsItemExistInQuickSlot(NewItemData, ItemExistInQuickSlotIndex))
	{
		DeleteItemFromQuickSlot(ItemExistInQuickSlotIndex);
	}
	
	// if Item Exist in NewQuickSlotIndex, delete item
	DeleteItemFromQuickSlot(NewQuickSlotIndex);

	QuickSlotItems.Add(NewQuickSlotIndex, NewItemData);

	//BroadCast to Inventory Widget
	if (InventoryComp->ItemRegisterDelegate.IsBound())
	{
		InventoryComp->ItemRegisterDelegate.Broadcast(NewItemData, true);
	}

	//BroadCast to QuickSlot Widget
	if (QuickSlotUpdateDelegate.IsBound())
	{
		QuickSlotUpdateDelegate.Broadcast(NewQuickSlotIndex, NewItemData, NewItemAmount);
	}
}

void UQuickSlotComponent::DeleteItemFromQuickSlot(const int32& NewQuickSlotIndex)
{
	if (!QuickSlotItems.Contains(NewQuickSlotIndex))
	{
		return;
	}

	UItemData* DeleteItemData = QuickSlotItems[NewQuickSlotIndex];

	QuickSlotItems.Remove(NewQuickSlotIndex);

	//BroadCast to Inventory Widget
	if (InventoryComp->ItemRegisterDelegate.IsBound())
	{
		InventoryComp->ItemRegisterDelegate.Broadcast(DeleteItemData, false);
	}

	//BroadCast to QuickSlot Widget
	if (QuickSlotUpdateDelegate.IsBound())
	{
		QuickSlotUpdateDelegate.Broadcast(NewQuickSlotIndex, nullptr, 0);
	}
}

bool UQuickSlotComponent::IsItemExistInQuickSlot(UItemData* TargetItemData, int32& OutIndex)
{
	for (auto& MyQuickslot : QuickSlotItems)
	{
		if (MyQuickslot.Value == TargetItemData)
		{
			OutIndex = MyQuickslot.Key;

			return true;
		}
	}

	return false;
}

int32 UQuickSlotComponent::GetEmptyQuickSlotSlotIndex()
{
	int32 EmptyIndex = -99;

	// Max Slot Length is 6
	for (int i = 0; i < 6; i++)
	{
		if (!QuickSlotItems.Contains(i))
		{
			EmptyIndex = i;

			break;
		}
	}

	return EmptyIndex;
}
