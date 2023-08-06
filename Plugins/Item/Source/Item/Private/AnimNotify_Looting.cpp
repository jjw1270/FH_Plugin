// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Looting.h"
#include "Item_PlayableCharacter.h"
#include "Item_FHPlayerController.h"
#include "InventoryComponent.h"

UAnimNotify_Looting::UAnimNotify_Looting(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldFireInEditor = false;
}

void UAnimNotify_Looting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(MeshComp->GetOwner()->GetInstigatorController());
	ensureMsgf(PC, TEXT("PC is nullptr"));

	PC->GetInventoryComp()->AddItemToInventory(GetRandomItemOnItemDropTable(), 1);

	DestroyLootItem(MeshComp->GetOwner());
}

int32 UAnimNotify_Looting::GetRandomItemOnItemDropTable()
{
	FItemDropData* ItemDropData = GetItemDropData(201);  //Should Set Params Later!
	ensureMsgf(ItemDropData, TEXT("ItemDropTable is nullptr"));

	// Sort ItemDropMap by Weight
	ItemDropData->ItemDropWeightsMap.ValueSort([](const int32& A, const int32& B) { return A < B; });

	// Random Item By Weights
	int32 RandomInt = FMath::RandRange(1, GetTotalItemDropWeight(ItemDropData->ItemDropWeightsMap));

	int32 ItemID = 0;
	for (const auto& DropWeightByItem : ItemDropData->ItemDropWeightsMap)
	{
		ItemID = DropWeightByItem.Key;
		if (RandomInt <= DropWeightByItem.Value)
		{
			break;
		}
	}

	return ItemID;
}

FItemDropData* UAnimNotify_Looting::GetItemDropData(const int32& DungeonID)
{
	ensureMsgf(IsValid(ItemDropDataTable), TEXT("ItemDropDataTable is nullptr"));

	return ItemDropDataTable->FindRow<FItemDropData>(*FString::FromInt(DungeonID), TEXT(""));
}

int32 UAnimNotify_Looting::GetTotalItemDropWeight(TMap<int32, int32>& InItemDropWeightsMap)
{
	int32 TotalItemDropWeights = 0;
	for (const auto& ItemDropWeight : InItemDropWeightsMap)
	{
		TotalItemDropWeights += ItemDropWeight.Value;
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
