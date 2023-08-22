// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHGameInstance.h"
#include "ItemDataManager.h"

UItem_FHGameInstance::UItem_FHGameInstance()
{
	if (ItemDataManagerClass)
	{
		ItemDataManager = NewObject<UItemDataManager>(this, ItemDataManagerClass);
	}
}

void UItem_FHGameInstance::SetCurrentDungeonID(const int32& DungeonID)
{
	CurrentDungeonID = DungeonID;
}

//void UItem_FHGameInstance::TESTss()
//{
//	for (auto& a : Equiments)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("%d"), a.Value);
//	}
//}
