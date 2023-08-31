// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHPlayerState.h"
#include "Item.h"
#include "Net/UnrealNetwork.h"
#include "Item_FHGameInstance.h"
#include "Item_FHPlayerController.h"
#include "EquipmentComponent.h"
#include "ItemData.h"
#include "Math/UnrealMathUtility.h"

#include "Kismet/GameplayStatics.h"

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
	AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(GetOwningController());
	CHECK_VALID(PC);

	if (PC != UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		return;
	}

	GI = GetGameInstance<UItem_FHGameInstance>();
	CHECK_VALID(GI);

	InitCurrentPlayerStats();

	UEquipmentComponent* EquipmentComp = PC->GetEquipmentComp();
	CHECK_VALID(EquipmentComp);

	EquipmentComp->WeaponUpdateDelegate.AddUObject(this, &AItem_FHPlayerState::OnWeaponUpdate);
	EquipmentComp->ArmorUpdateDelegate.AddUObject(this, &AItem_FHPlayerState::OnArmorUpdate);
}

void AItem_FHPlayerState::InitCurrentPlayerStats()
{
	UE_LOG(LogTemp, Warning, TEXT("InitCurrentPlayerStats"));

	CurrentHealth = GI->GetDefaultPlayerStats().DefaultHealth;
	CurrentStamina = GI->GetDefaultPlayerStats().DefaultStamina;
	CurrentAttack = GI->GetDefaultPlayerStats().DefaultAttack;
	CurrentAttackSpeed = GI->GetDefaultPlayerStats().DefaultAttackSpeed;
	CurrentCritcal = GI->GetDefaultPlayerStats().DefaultCritcal;
	CurrentDefence = GI->GetDefaultPlayerStats().DefaultDefence;
}

void AItem_FHPlayerState::OnWeaponUpdate(UItemData* UpdateEquipItem, const bool& bIsEquip)
{
	FWeaponItemData UpdateWeaponItemData;
	if (!UpdateEquipItem->GetWeaponData(UpdateWeaponItemData))
	{
		return;
	}

	UpdateDefaultPlayerStats(bIsEquip, 0, 0,
		UpdateWeaponItemData.AttackPower, UpdateWeaponItemData.AttackSpeed, UpdateWeaponItemData.CriticalChance, 0);
}

void AItem_FHPlayerState::OnArmorUpdate(const EArmorType& UpdateArmorType, UItemData* UpdateEquipItem, const bool& bIsEquip)
{
	FArmorItemData UpdateArmorItemData;
	if (!UpdateEquipItem->GetArmorData(UpdateArmorItemData))
	{
		return;
	}

	UpdateDefaultPlayerStats(bIsEquip, UpdateArmorItemData.Health, UpdateArmorItemData.Stamina, UpdateArmorItemData.AttackPower,
		UpdateArmorItemData.AttackSpeed, UpdateArmorItemData.CriticalChance, UpdateArmorItemData.DefensivePower);

}

void AItem_FHPlayerState::UpdateDefaultPlayerStats(const bool& bIsEquip, const int32& AddHealth, const int32& AddStamina, const int32& AddAttack, const float& AddAttackSpeed, const float& AddCritcal, const int32& AddDefence)
{
	if (bIsEquip)
	{
		GI->GetDefaultPlayerStats().DefaultHealth += AddHealth;
		GI->GetDefaultPlayerStats().DefaultStamina += AddStamina;
		GI->GetDefaultPlayerStats().DefaultAttack += AddAttack;
		GI->GetDefaultPlayerStats().DefaultAttackSpeed += AddAttackSpeed;
		GI->GetDefaultPlayerStats().DefaultCritcal += AddCritcal;
		GI->GetDefaultPlayerStats().DefaultDefence += AddDefence;
	}
	else
	{
		GI->GetDefaultPlayerStats().DefaultHealth -= AddHealth;
		GI->GetDefaultPlayerStats().DefaultStamina -= AddStamina;
		GI->GetDefaultPlayerStats().DefaultAttack -= AddAttack;
		GI->GetDefaultPlayerStats().DefaultAttackSpeed -= AddAttackSpeed;
		GI->GetDefaultPlayerStats().DefaultCritcal -= AddCritcal;
		GI->GetDefaultPlayerStats().DefaultDefence -= AddDefence;
	}

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
