// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "ItemType.generated.h"

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

USTRUCT(Atomic, BlueprintType)
struct FConsumableItemData : public FTableRowBase
{
    GENERATED_BODY()

    public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Price;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEffectSelf;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bHasDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxEquipNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemTextInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* ItemImage;
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
    Helmet      UMETA(DisplayName = "Helmet"),
    Upper       UMETA(DisplayName = "Upper"),
    Lower       UMETA(DisplayName = "Lower"),
    Shoes       UMETA(DisplayName = "Shoes"),
    Weapon      UMETA(DisplayName = "Weapon"),
};

USTRUCT(Atomic, BlueprintType)
struct FEquipmentItemData : public FTableRowBase
{
    GENERATED_BODY()

    public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Price;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEquipmentType EquipmentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Stamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CriticalChance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefensivePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemTextInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* ItemImage;
};

// Inventory ItemType
UENUM(BlueprintType)
enum class EItemType : uint8
{
    None            UMETA(DisplayName = "None"),
    Consumable      UMETA(DisplayName = "Consumable"),
    Equipment       UMETA(DisplayName = "Equipment"),
};
