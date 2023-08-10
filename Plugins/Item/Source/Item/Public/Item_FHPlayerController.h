// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "Item_FHPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ITEM_API AItem_FHPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AItem_FHPlayerController();

// Inherited Functions
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

// Player Input
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* UIMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* InventoryAction;

// Components
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	class UInventoryComponent* InventoryComp;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UInventoryComponent* GetInventoryComp() const { return InventoryComp; }

// UI
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UItem_HUDWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	class UItem_HUDWidget* HUDWidget;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UItem_HUDWidget* GetHUDWidget() const { return HUDWidget; }

};
