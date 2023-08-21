// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Looting.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UAnimNotify_Looting : public UAnimNotify
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* ItemDropDataTable;
	
protected:
	int32 GetRandomItemOnItemDropTable();

	struct FItemDropData* GetItemDropData(const int32& DungeonID);

	int32 GetTotalItemDropWeight(TMap<int32, int32>& InItemDropWeightsMap);

	void DestroyLootItem(AActor* Owner);

};
