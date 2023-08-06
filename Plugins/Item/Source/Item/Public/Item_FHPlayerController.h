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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	class UInventoryComponent* InventoryComp;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UInventoryComponent* GetInventoryComp() const { return InventoryComp; }

//Widget
protected:
	bool bIsInventoryUIOpen;

public:
	UFUNCTION(BlueprintCallable)
	void InventoryUI();

};
