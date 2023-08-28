// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "ItemDataManager.h"
#include "ModularSkeletalMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API UModularSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	UModularSkeletalMeshComponent(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void BeginPlay() override;

public:
	void InitDefaultSkeletalMesh();

	UFUNCTION(BlueprintCallable)
	void SetEquipMesh(USkeletalMesh* NewArmorMesh, const bool& bIsEquip);

	UFUNCTION(Server, Reliable)
	void Req_SetEquipMesh(USkeletalMesh* NewArmorMesh, const bool bIsEquip);

	UFUNCTION(NetMulticast, Reliable)
	void Res_SetEquipMesh(USkeletalMesh* NewArmorMesh, const bool bIsEquip);

protected:
	EArmorType ArmorType;

	UPROPERTY()
	USkeletalMesh* DefaultSkeletalMeshAsset;

	bool bHideHairWhenVisible;

public:
	FORCEINLINE void SetArmorType(EArmorType NewArmorType) { ArmorType = NewArmorType; }

	FORCEINLINE const EArmorType& GetArmorType() { return ArmorType; }

	FORCEINLINE void SetbHideHairWhenVisible(bool bValue) { bHideHairWhenVisible = bValue; }

	FORCEINLINE const bool& GetbHideHairWhenVisible() { return bHideHairWhenVisible; }
};
