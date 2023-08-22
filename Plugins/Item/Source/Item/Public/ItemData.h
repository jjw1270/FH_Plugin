// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataManager.h"
#include "ItemData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ITEM_API UItemData : public UObject
{
	GENERATED_BODY()
	
public:
	UItemData();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FConsumableItemData ConsumableData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FWeaponItemData WeaponData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FArmorItemData ArmorData;

	// UniqueID for Prevent Item Data Overlaping ( 1001 ~ 9999 )
	// Only for NonStackable Items!
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 UniqueID;

public:
	void SetConsumableData(FConsumableItemData& NewConsumableData);

	void SetWeaponData(FWeaponItemData& NewWeaponData, int32 NewUniqueID);

	void SetArmorData(FArmorItemData& NewArmorData, int32 NewUniqueID);

public:
	UFUNCTION(BlueprintCallable)
	bool GetBaseData(FBaseItemData& OutData);

	UFUNCTION(BlueprintCallable)
	EItemType GetItemType();

	UFUNCTION(BlueprintCallable)
	bool GetConsumableData(FConsumableItemData& OutData);

	UFUNCTION(BlueprintCallable)
	bool GetWeaponData(FWeaponItemData& OutData);

	UFUNCTION(BlueprintCallable)
	bool GetArmorData(FArmorItemData& OutData);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetUniqueID() const { return UniqueID; }

};
