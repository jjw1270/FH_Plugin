// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHPlayerState.h"
#include "Item.h"
#include "Item_FHGameInstance.h"
#include "Net/UnrealNetwork.h"

AItem_FHPlayerState::AItem_FHPlayerState()
{

}

void AItem_FHPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const

{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem_FHPlayerState, UserName);

}

void AItem_FHPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority() && GetLocalRole() != ENetRole::ROLE_AutonomousProxy)
	{
		return;
	}

	UItem_FHGameInstance* GI = GetGameInstance<UItem_FHGameInstance>();
	CHECK_VALID(GI);

	int32 RandomID = FMath::RandRange(100, 999);

	SetUserName(FText::AsNumber(RandomID));
}

void AItem_FHPlayerState::SetUserName(const FText& NewName)
{
	UserName = NewName;
}

void AItem_FHPlayerState::OnRep_UserName()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SetUserName %s"), *UserName.ToString()));

	// broadcast for nametag
}
