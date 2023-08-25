// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Item_PlayableCharacter.generated.h"

UCLASS()
class ITEM_API AItem_PlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AItem_PlayableCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

protected:
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	virtual void Move(const FInputActionValue& Value);   //Temp Virtual for develop

	/** Called for looking input */
	virtual void Look(const FInputActionValue& Value);   //Temp Virtual for develop


////////////////////////////
////////////////////////////
////////////////////////////

/*
Modular Mesh Components
<LowerBody>
	<Shoes>
	<UpperBody>
		<Back>
		<Glove_L>
			<Weapon_L>
		<Glove_R>
			<Weapon_R>
		<Head>
			<Hair>
			<Helmet>
*/ 

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	class USkeletalMeshComponent* Shoes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	class USkeletalMeshComponent* UpperBody;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	class USkeletalMeshComponent* Glove_L;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	class USkeletalMeshComponent* Glove_R;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	class USkeletalMeshComponent* Weapon_R;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	class USkeletalMeshComponent* Head;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	class USkeletalMeshComponent* Hair;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	class USkeletalMeshComponent* Helmet;



// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<UCapsuleComponent> InteractCollision;

protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	/** interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* InteractAction;

	// Quick Slot Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* QuickSlot1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* QuickSlot2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* QuickSlot3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* QuickSlot4Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* QuickSlot5Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* QuickSlot6Action;

protected:
	/** Called for Interact input */
	void Interaction(const FInputActionValue& Value);

	/** Called for Quick Slot inputs */
	void UseQuickSlot(int32 SlotNum);

protected:
	UFUNCTION()
	void OnWeaponUpdate(class UItemData* UpdateEquipItem, const bool& bIsEquip);

	UFUNCTION()
	void OnArmorUpdate(const EArmorType& UpdateArmorType, class UItemData* UpdateEquipItem, const bool& bIsEquip);

protected:
	UPROPERTY()
	class AItem_FHPlayerController* PC;

	UPROPERTY()
	class UQuickSlotComponent* QuickSlotComp;

public:
	UPROPERTY(BlueprintReadOnly)
	AActor* InteractingActor;  // For AnimNotify on Interaction Montage Playing, Delete when Notify End Execution.

	UPROPERTY(BlueprintReadWrite)
	AActor* LootingEffect;
};
