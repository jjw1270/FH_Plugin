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

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* UIMappingContext;

	/** Inventory Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* InventoryAction;
protected:
	bool bIsInventoryUIOpen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	class UInventoryWidget* InventoryWidget;

public:
	UFUNCTION(BlueprintCallable)
	void InventoryUI();

};
