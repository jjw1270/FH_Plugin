// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Looting.h"
#include "Item_PlayableCharacter.h"

#include "Item_FHPlayerController.h"
#include "InventoryComponent.h"

UAnimNotify_Looting::UAnimNotify_Looting()
{
	bShouldFireInEditor = false;
}

void UAnimNotify_Looting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(MeshComp->GetOwner()->GetInstigatorController());
	ensureMsgf(PC, TEXT("PC is nullptr"));

	PC->GetInventoryComponent()->AddItemToInventory(GetRandomItemOnItemDropTable(), 1);

	DestroyLootItem(MeshComp->GetOwner());
}

int32 UAnimNotify_Looting::GetRandomItemOnItemDropTable()
{
	FItemDropData* ItemDropTable = GetItemDropData(1);  //Should Set Params Later!
	ensureMsgf(ItemDropTable, TEXT("ItemDropTable is nullptr"));

	// Sort ItemTable by Weight
	ItemDropTable->DropWeightByItemArray.Sort([](const FDropWeightByItem& A, const FDropWeightByItem& B) { return A.DropWeight < B.DropWeight; });

	// Random Item By Weights
	int32 RandomInt = FMath::RandRange(1, GetTotalItemDropWeight(ItemDropTable->DropWeightByItemArray));

	int32 ItemID = 0;
	for (auto& DropWeightByItem : ItemDropTable->DropWeightByItemArray)
	{
		ItemID = DropWeightByItem.ItemID;
		if (RandomInt <= DropWeightByItem.DropWeight)
		{
			break;
		}
	}

	return ItemID;
}

FItemDropData* UAnimNotify_Looting::GetItemDropData(const int32& DungeonID)
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
