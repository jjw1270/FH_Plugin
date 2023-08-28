// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularSkeletalMeshComponent.h"
#include "Item.h"
#include "Net/UnrealNetwork.h"

UModularSkeletalMeshComponent::UModularSkeletalMeshComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// SetIsReplicatedByDefault(true);
}

void UModularSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	InitDefaultSkeletalMesh();
}

void UModularSkeletalMeshComponent::InitDefaultSkeletalMesh()
{
	DefaultSkeletalMeshAsset = GetSkeletalMeshAsset();

	// UE_LOG(LogTemp, Error, TEXT("YOU SHOULD SET SKELETAL MESH ASSET! : %d"), *GetName());
}

void UModularSkeletalMeshComponent::SetEquipMesh(USkeletalMesh* NewArmorMesh, const bool& bIsEquip)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SetEquipMesh")));

	// if UnEquip, Set default Mesh
	if (!bIsEquip)
	{
		SetSkeletalMeshAsset(DefaultSkeletalMeshAsset);

		return;
	}

	CHECK_VALID(NewArmorMesh);
	SetSkeletalMeshAsset(NewArmorMesh);

	// Req_SetEquipMesh(NewArmorMesh, bIsEquip);
}

void UModularSkeletalMeshComponent::Req_SetEquipMesh_Implementation(USkeletalMesh* NewArmorMesh, const bool bIsEquip)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Req : SetEquipMesh")));

	Res_SetEquipMesh(NewArmorMesh, bIsEquip);
}

void UModularSkeletalMeshComponent::Res_SetEquipMesh_Implementation(USkeletalMesh* NewArmorMesh, const bool bIsEquip)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Res : SetEquipMesh")));
	
	// if UnEquip, Set default Mesh
	if (!bIsEquip)
	{
		SetSkeletalMeshAsset(DefaultSkeletalMeshAsset);

		return;
	}

	CHECK_VALID(NewArmorMesh);
	SetSkeletalMeshAsset(NewArmorMesh);
}
