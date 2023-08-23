// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuickSlotComponent.generated.h"


// Delegate called when an QuickSlot Item Changed
// const int32& QuickSlotIndex, UItemData* ItemData, const int32& ItemAmount
DECLARE_MULTICAST_DELEGATE_ThreeParams(FDele_Multi_QuickSlotUpdate, const int32&, class UItemData*, const int32&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITEM_API UQuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UQuickSlotComponent();

protected:
	virtual void BeginPlay() override;

protected:
	void InitComponent();
		
protected:
	UPROPERTY()
	class UInventoryComponent* InventoryComp;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<int32, class UItemData*> QuickSlotItems;

// UI Delegate
public:
	FDele_Multi_QuickSlotUpdate QuickSlotUpdateDelegate;

public:
	UFUNCTION(BlueprintCallable)
	bool IsItemExistInQuickSlot(class UItemData* TargetItemData, int32& OutIndex);

	UFUNCTION(BlueprintCallable)
	void SetItemToQuickSlot(const int32& NewQuickSlotIndex, class UItemData* NewItemData, const int32& NewItemAmount);

	UFUNCTION(BlueprintCallable)
	void DeleteItemFromQuickSlot(const int32& NewQuickSlotIndex);

	UFUNCTION(BlueprintCallable)
	int32 GetEmptyQuickSlotSlotIndex();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TMap<int32, class UItemData*>& GetQuickSlotItems() { return QuickSlotItems; }

public:
	UFUNCTION(BlueprintCallable)
	void UseQuickSlotItem(int32 TargetQuickSlotIndex);
};
