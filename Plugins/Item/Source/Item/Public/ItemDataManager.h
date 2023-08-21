// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ItemDataManager.generated.h"

// Item drop data by dungeon struct
USTRUCT(Atomic, BlueprintType)
struct FItemDropData : public FTableRowBase
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DungeonID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DungeonName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, int32> ItemDropWeightsMap;
};

UCLASS()
class ITEM_API UItemDataManager : public UObject
{
	GENERATED_BODY()
	
public:
	UItemDataManager();

//Item DataTables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* ConsumableItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* EquipmentItemDataTable;

public:
	struct FConsumableItemData* GetConsumableItemInfo(const int32& ItemID);

	struct FEquipmentItemData* GetEquipmentItemInfo(const int32& ItemID);

};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None            UMETA(DisplayName = "None"),
	Consumable      UMETA(DisplayName = "Consumable"),
	Weapon			UMETA(DisplayName = "Weapon"),
	Armor			UMETA(DisplayName = "Armor"),
	Others			UMETA(DisplayName = "Others"),
};

USTRUCT(BlueprintType)
struct FBaseItemData
{
	GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BasePrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UpgradeValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemTextInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon2D;
};

UENUM(BlueprintType)
enum class EEffectTarget : uint8
{
	None				UMETA(DisplayName = "None"),
	Health				UMETA(DisplayName = "Health"),
	Stamina				UMETA(DisplayName = "Stamina"),
	AttackPower			UMETA(DisplayName = "AttackPower"),
	AttackSpeed			UMETA(DisplayName = "AttackSpeed"),
	CriticalChance		UMETA(DisplayName = "CriticalChance"),
	DefensivePower		UMETA(DisplayName = "DefensivePower"),
};

USTRUCT(BlueprintType)
struct FConsumableItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBaseItemData BaseData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEffectSelf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectTarget EffectTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectValue;

};

USTRUCT(BlueprintType)
struct FWeaponItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBaseItemData BaseData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StaminaUsage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseAttackPower;

	// Default : 1.0f, Max : 2.5f
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAttackSpeed;

	// Max : 1.0f
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseCriticalChance;
};


UENUM(BlueprintType)
enum class EArmorType : uint8
{
	None        UMETA(DisplayName = "None"),
	Helmet      UMETA(DisplayName = "Helmet"),
	Upper       UMETA(DisplayName = "Upper"),
	Gloves		UMETA(DisplayName = "Gloves"),
	Lower       UMETA(DisplayName = "Lower"),
	Shoes       UMETA(DisplayName = "Shoes"),

};


USTRUCT(BlueprintType)
struct FArmorItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBaseItemData BaseData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EArmorType ArmorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackPower;

	// Default : 1.0f, Max : 2.5f
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed;

	// Max : 1.0f
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DefensivePower;
};
