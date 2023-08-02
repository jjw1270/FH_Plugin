// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ItemInterface.h"
#include "AnimNotify_Looting.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UAnimNotify_Looting : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_Looting();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* ItemDropDataTable;
	
protected:
	FString GetRandomItemNameOnItemDropTable();

	struct FItemDropData* GetItemDropData(int32 DungeonID);

	int32 GetTotalItemDropWeight(const TArray<FDropWeightByItem>& DropWeightsByItem);

	void DestroyLootItem(AActor* Owner);

};
