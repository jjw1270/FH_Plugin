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

//protected:
//	UPROPERTY(ReplicatedUsing = OnRep_UserName)
//	FString UserName;
//
//public:
//	UFUNCTION(BlueprintCallable)
//	void SetUserName(const FString& NewName);
//
//	UFUNCTION(BlueprintPure)
//	FORCEINLINE FString& GetUserName() { return UserName; };
//
//	UFUNCTION()
//	void OnRep_UserName();

};
