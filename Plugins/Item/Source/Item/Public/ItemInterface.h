// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/DataTable.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ITEM_API IItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventLoot(ACharacter* OwnCharacter);
};



// Structs and Enums that using in Item Plugin //

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
    TObjectPtr<UTexture2D> ItemImage;
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
    TObjectPtr<UTexture2D> ItemImage;
};

USTRUCT(Atomic, BlueprintType)
struct FDropWeightByItem
{
    GENERATED_BODY()

    public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DropWeight;
};

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
    TArray<FDropWeightByItem> DropWeightsByItem;
};
