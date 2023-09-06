// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Item_FHPlayerState.generated.h"

/**
 * 
 */

UCLASS()
class ITEM_API AItem_FHPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AItem_FHPlayerState();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_UserName, Category = State)
	FText UserName;

public:
	UFUNCTION(BlueprintCallable, Category = State)
	void SetUserName(const FText& NewName);

	UFUNCTION(BlueprintPure, Category = State)
	FORCEINLINE FText& GetUserName() { return UserName; };

	UFUNCTION()
	void OnRep_UserName();

};
