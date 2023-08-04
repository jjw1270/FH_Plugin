// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "DropItem.generated.h"

UCLASS()
class ITEM_API ADropItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<class USphereComponent> RootSphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<class UBoxComponent> OverlapBoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<class UWidgetComponent> LootWidgetComp;

private:
	APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAnimMontage> LootingMontage;

	UPROPERTY(EditAnywhere, Category = Widget, Meta = (AllowPrivateAccess = true))
	float WidgetVisiblityRange{ 1500.f };

	FTimerHandle UpdateWidgetVisiblityTimerHandle;

	bool bWidgetVisibility;

protected:
	void UpdateWidgetVisiblity();

	bool IsCanSeePlayer();

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventLoot(ACharacter* OwnCharacter);

	virtual void EventLoot_Implementation(ACharacter* OwnCharacter) override;

};
