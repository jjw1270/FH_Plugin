// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_FHPlayerState.h"
#include "EquipmentComponent.h"

AItem_FHPlayerState::AItem_FHPlayerState()
{
	EquipmentComp = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComp"));
}
