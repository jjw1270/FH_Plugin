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
	class UInputAction* InventoryUIAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* EquipmentUIAction;

// Input Func
public:
	UFUNCTION(BlueprintCallable)
	void WidgetOnOff(FName WidgetName);

// Components
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class UInventoryComponent* InventoryComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class UQuickSlotComponent* QuickSlotComp;

// Component Getter
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UInventoryComponent* GetInventoryComp() const { return InventoryComp; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UQuickSlotComponent* GetQuickSlotComp() const { return QuickSlotComp; }

// UI
protected:
	UPROPERTY()
	class UItem_HUDWidget* HUDWidget;

};
