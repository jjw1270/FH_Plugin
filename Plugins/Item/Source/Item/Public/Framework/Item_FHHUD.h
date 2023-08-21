// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Item_FHHUD.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API AItem_FHHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UItem_HUDWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	class UItem_HUDWidget* HUDWidget;

public:
	UFUNCTION(BlueprintCallable)
	class UItem_HUDWidget* GetHUDWidget();

};
