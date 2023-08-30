// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHPlayerState.h"
#include "Item.h"
#include "Net/UnrealNetwork.h"
#include "Item_FHPlayerController.h"
#include "EquipmentComponent.h"
#include "ItemData.h"
#include "Math/UnrealMathUtility.h"

AItem_FHPlayerState::AItem_FHPlayerState()
{
	

}

void AItem_FHPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem_FHPlayerState, CurrentHealth);
}

void AItem_FHPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Bind Equipment Update Delegates
	// Only for Local client
	AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(GetPlayerController());
	CHECK_VALID(PC);

	UEquipmentComponent* EquipmentComp = PC->GetEquipmentComp();
	CHECK_VALID(EquipmentComp);

	EquipmentComp->WeaponUpdateDelegate.AddUObject(this, &AItem_FHPlayerState::OnWeaponUpdate);
	EquipmentComp->ArmorUpdateDelegate.AddUObject(this, &AItem_FHPlayerState::OnArmorUpdate);
}

void AItem_FHPlayerState::OnWeaponUpdate(UItemData* UpdateEquipItem, const bool& bIsEquip)
{
	FWeaponItemData UpdateWeaponItemData;
	if (!UpdateEquipItem->GetWeaponData(UpdateWeaponItemData))
	{
		return;
	}

	UpdateDefaultPlayerStats(0, 0,
		UpdateWeaponItemData.AttackPower, UpdateWeaponItemData.AttackSpeed, UpdateWeaponItemData.CriticalChance, 0);
}

void AItem_FHPlayerState::OnArmorUpdate(const EArmorType& UpdateArmorType, UItemData* UpdateEquipItem, const bool& bIsEquip)
{
	FArmorItemData UpdateArmorItemData;
	if (!UpdateEquipItem->GetArmorData(UpdateArmorItemData))
	{
		return;
	}

	UpdateDefaultPlayerStats(UpdateArmorItemData.Health, UpdateArmorItemData.Stamina, UpdateArmorItemData.AttackPower,
		UpdateArmorItemData.AttackSpeed, UpdateArmorItemData.CriticalChance, UpdateArmorItemData.DefensivePower);

}

void AItem_FHPlayerState::UpdateDefaultPlayerStats(int32 AddHealth, int32 AddStamina, int32 AddAttack, float AddAttackSpeed, float AddCritcal, int32 AddDefence)
{
	DefaultPlayerStats.DefaultHealth += AddHealth;
	DefaultPlayerStats.DefaultStamina += AddStamina;
	DefaultPlayerStats.DefaultAttack += AddAttack;
	DefaultPlayerStats.DefaultAttackSpeed += AddAttackSpeed;
	DefaultPlayerStats.DefaultCritcal += AddCritcal;
	DefaultPlayerStats.DefaultDefence += AddDefence;

	// Save DefaultPlayerStats Data in Instance
}

void AItem_FHPlayerState::OnRep_CurrentHealth()
{
	// broadcast for nametag
}

void AItem_FHPlayerState::SetUserName(const FString& NewName)
{
	UserName = NewName;

	OnRep_UserName();
}

void AItem_FHPlayerState::OnRep_UserName()
{
	// broadcast for nametag
}
