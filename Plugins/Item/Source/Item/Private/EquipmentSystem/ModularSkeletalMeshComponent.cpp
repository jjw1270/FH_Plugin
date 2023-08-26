// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularSkeletalMeshComponent.h"
#include "Item.h"

void UModularSkeletalMeshComponent::BeginPlay()
{
	InitDefaultSkeletalMesh();
}

void UModularSkeletalMeshComponent::InitDefaultSkeletalMesh()
{
	DefaultSkeletalMeshAsset = GetSkeletalMeshAsset();

	// UE_LOG(LogTemp, Error, TEXT("YOU SHOULD SET SKELETAL MESH ASSET! : %d"), *GetName());
}

void UModularSkeletalMeshComponent::SetEquipMesh(USkeletalMesh* NewArmorMesh, const bool& bIsEquip)
{
	CHECK_VALID(NewArmorMesh);

	// if UnEquip, Set default Mesh
	if (!bIsEquip)
	{
		SetSkeletalMeshAsset(DefaultSkeletalMeshAsset);

		return;
	}

	SetSkeletalMeshAsset(NewArmorMesh);
}
