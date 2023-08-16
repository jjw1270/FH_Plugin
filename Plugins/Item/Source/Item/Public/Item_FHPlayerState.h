// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Item_FHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API AItem_FHPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AItem_FHPlayerState();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	class UEquipmentComponent* EquipmentComp;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UEquipmentComponent* GetEquipmentComp() const { return EquipmentComp; }

};
