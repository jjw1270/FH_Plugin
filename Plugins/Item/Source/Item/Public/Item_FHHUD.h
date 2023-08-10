// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Item_FHHUD.generated.h"

/**
 * MainGameHUD
 */
UCLASS()
class ITEM_API AItem_FHHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;


};
