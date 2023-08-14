// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHGameInstance.h"
#include "InventoryComponent.h"

void UItem_FHGameInstance::TestInventory()
{
	for (auto a : InventoryItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), a.Key);
	}
}
