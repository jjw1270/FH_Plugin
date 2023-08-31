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
	UPROPERTY()
	class UItem_FHGameInstance* GI;
	
protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnWeaponUpdate(class UItemData* UpdateEquipItem, const bool& bIsEquip);

	UFUNCTION()
	void OnArmorUpdate(const EArmorType& UpdateArmorType, class UItemData* UpdateEquipItem, const bool& bIsEquip);

protected:
	UFUNCTION()
	void UpdateDefaultPlayerStats(const bool& bIsEquip, const int32& AddHealth, const int32& AddStamina, const int32& AddAttack, const float& AddAttackSpeed, const float& AddCritcal, const int32& AddDefence);

	void InitCurrentPlayerStats();

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentHealth)
	int32 CurrentHealth;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentStamina;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentAttack;

	UPROPERTY(BlueprintReadOnly)
	float CurrentAttackSpeed;

	UPROPERTY(BlueprintReadOnly)
	float CurrentCritcal;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentDefence;

public:
	UFUNCTION()
	void OnRep_CurrentHealth();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_UserName)
	FString UserName;

public:
	UFUNCTION(BlueprintCallable)
	void SetUserName(const FString& NewName);

	UFUNCTION(BlueprintPure)
	FORCEINLINE FString& GetUserName() { return UserName; };

	UFUNCTION()
	void OnRep_UserName();
};
