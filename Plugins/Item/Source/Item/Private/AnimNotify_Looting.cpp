// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Looting.h"
#include "Item_PlayableCharacter.h"

UAnimNotify_Looting::UAnimNotify_Looting()
{
	bShouldFireInEditor = false;
}

void UAnimNotify_Looting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	FString LootItemName = GetRandomItemNameOnItemDropTable();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
		FString::Printf(TEXT("Loot Item = %s"), *LootItemName));

	//Check Item Struct -> Consumalbe or Equipment
	//Add Item To Inventory

	/////////////////////

	DestroyLootItem(MeshComp->GetOwner());
}

FString UAnimNotify_Looting::GetRandomItemNameOnItemDropTable()
{
	FItemDropData* ItemDropTable = GetItemDropData(1);  //Should Set Params Later!
	ensureMsgf(ItemDropTable, TEXT("ItemDropTable is nullptr"));

	// Sort ItemTable by Weight
	ItemDropTable->DropWeightsByItem.Sort([](const FDropWeightByItem& A, const FDropWeightByItem& B) { return A.DropWeight < B.DropWeight; });

	// Random Item By Weights
	int32 RandomInt = FMath::RandRange(1, GetTotalItemDropWeight(ItemDropTable->DropWeightsByItem));

	FString ItemName;
	for (auto& DropWeightByItem : ItemDropTable->DropWeightsByItem)
	{
		ItemName = DropWeightByItem.Name;
		if (RandomInt <= DropWeightByItem.DropWeight)
		{
			break;
		}
	}

	return ItemName;
}

FItemDropData* UAnimNotify_Looting::GetItemDropData(int32 DungeonID)
{
	ensureMsgf(IsValid(ItemDropDataTable), TEXT("DT_ItemDropTableByDungeon is nullptr"));

	return ItemDropDataTable->FindRow<FItemDropData>(*FString::FromInt(DungeonID), TEXT(""));
}

int32 UAnimNotify_Looting::GetTotalItemDropWeight(const TArray<FDropWeightByItem>& DropWeightsByItem)
{
	int32 TotalItemDropWeights = 0;
	for (auto& DropWeightByItem : DropWeightsByItem)
	{
		TotalItemDropWeights += DropWeightByItem.DropWeight;
	}

	return TotalItemDropWeights;
}

void UAnimNotify_Looting::DestroyLootItem(AActor* Owner)
{
	if (AItem_PlayableCharacter* Char = Cast<AItem_PlayableCharacter>(Owner))
	{
		Char->InteractingActor->Destroy();
		Char->InteractingActor = nullptr;
	}
}
