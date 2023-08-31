// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_PlayableCharacter.h"
#include "Item.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//Enhanced Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InteractionInterface.h"
#include "Item_FHPlayerController.h"
#include "QuickSlotComponent.h"
#include "EquipmentComponent.h"

#include "ItemData.h"
#include "ModularSkeletalMeshComponent.h"

AItem_PlayableCharacter::AItem_PlayableCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(ACharacter::MeshComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 100.f);

	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Set Modular Meshes
	InitModularMeshComp();

	// Interact Collision
	InteractCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->SetCapsuleSize(60.f, 120.f);
}

void AItem_PlayableCharacter::InitModularMeshComp()
{
	LowerBody = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("LowerBody"));
	LowerBody->SetupAttachment(RootComponent);
	LowerBody->SetArmorType(EArmorType::Lower);
	ArmorMSMCompArray.Add(LowerBody);

	Shoes = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Shoes"));
	Shoes->SetupAttachment(LowerBody);
	Shoes->SetArmorType(EArmorType::Shoes);
	Shoes->SetLeaderPoseComponent(LowerBody);
	ArmorMSMCompArray.Add(Shoes);

	UpperBody = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("UpperBody"));
	UpperBody->SetupAttachment(LowerBody);
	UpperBody->SetArmorType(EArmorType::Upper);
	UpperBody->SetLeaderPoseComponent(LowerBody);
	ArmorMSMCompArray.Add(UpperBody);

	Cloak = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Cloak"));
	Cloak->SetupAttachment(UpperBody);
	Cloak->SetArmorType(EArmorType::None);
	Cloak->SetLeaderPoseComponent(LowerBody);

	Glove_L = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Glove_L"));
	Glove_L->SetupAttachment(UpperBody);
	Glove_L->SetArmorType(EArmorType::Gloves);
	Glove_L->SetLeaderPoseComponent(LowerBody);
	ArmorMSMCompArray.Add(Glove_L);

	Glove_R = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Glove_R"));
	Glove_R->SetupAttachment(UpperBody);
	Glove_R->SetArmorType(EArmorType::None);
	Glove_R->SetLeaderPoseComponent(LowerBody);

	Head = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(UpperBody);
	Head->SetArmorType(EArmorType::None);
	Head->SetLeaderPoseComponent(LowerBody);

	Hair = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(Head);
	Hair->SetArmorType(EArmorType::None);
	Hair->SetLeaderPoseComponent(LowerBody);

	Helmet = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Helmet"));
	Helmet->SetupAttachment(Head, TEXT("HAIR"));
	Helmet->SetArmorType(EArmorType::Helmet);
	ArmorMSMCompArray.Add(Helmet);

	Weapon = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(Glove_R, TEXT("WEAPON_R"));
	Weapon->SetArmorType(EArmorType::None);
}

void AItem_PlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Only run in Local Client
	//if (!HasAuthority() && GetLocalRole() != ROLE_AutonomousProxy)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Not local"));

	//	return;
	//}

	PC = Cast<AItem_FHPlayerController>(GetController());
	CHECK_VALID(PC);

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	QuickSlotComp = PC->GetQuickSlotComp();
	CHECK_VALID(QuickSlotComp);

	UEquipmentComponent* EquipmentComp = PC->GetEquipmentComp();
	CHECK_VALID(EquipmentComp);

	// Bind Equipment Update Delegates
	EquipmentComp->WeaponUpdateDelegate.AddUObject(this, &AItem_PlayableCharacter::OnWeaponUpdate);
	EquipmentComp->ArmorUpdateDelegate.AddUObject(this, &AItem_PlayableCharacter::OnArmorUpdate);

	EquipVisibilityUpdateDelegate.AddDynamic(this, &AItem_PlayableCharacter::OnEquipVisibilityUpdate);
}

void AItem_PlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AItem_PlayableCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AItem_PlayableCharacter::Look);

		//Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AItem_PlayableCharacter::Interaction);

		//Quick Slots
		EnhancedInputComponent->BindAction<AItem_PlayableCharacter, int32>(QuickSlot1Action, ETriggerEvent::Started, this, &AItem_PlayableCharacter::UseQuickSlot, 1);
		EnhancedInputComponent->BindAction<AItem_PlayableCharacter, int32>(QuickSlot2Action, ETriggerEvent::Started, this, &AItem_PlayableCharacter::UseQuickSlot, 2);
		EnhancedInputComponent->BindAction<AItem_PlayableCharacter, int32>(QuickSlot3Action, ETriggerEvent::Started, this, &AItem_PlayableCharacter::UseQuickSlot, 3);
		EnhancedInputComponent->BindAction<AItem_PlayableCharacter, int32>(QuickSlot4Action, ETriggerEvent::Started, this, &AItem_PlayableCharacter::UseQuickSlot, 4);
		EnhancedInputComponent->BindAction<AItem_PlayableCharacter, int32>(QuickSlot5Action, ETriggerEvent::Started, this, &AItem_PlayableCharacter::UseQuickSlot, 5);
		EnhancedInputComponent->BindAction<AItem_PlayableCharacter, int32>(QuickSlot6Action, ETriggerEvent::Started, this, &AItem_PlayableCharacter::UseQuickSlot, 6);
	}
}

void AItem_PlayableCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AItem_PlayableCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AItem_PlayableCharacter::Interaction(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Interaction"));

	// return if interacting is executing
	if (InteractingActor)
	{
		return;
	}

	TArray<AActor*> OverlappingActors;
	InteractCollision->GetOverlappingActors(OverlappingActors);

	// Find Nearest InterfaceObj
	double NearestLength = 9999.0f;

	for (AActor* Actor : OverlappingActors)
	{
		double distance = FVector::Dist(Actor->GetActorLocation(), GetActorLocation());
		if (NearestLength < distance)
		{
			continue;
		}

		// Check If Actor Inherits any Interfaces
		if (Cast<IInteractionInterface>(Actor))
		{
			NearestLength = distance;
			InteractingActor = Actor;
		}
	}

	if (IInteractionInterface* NearestItemInterfaceObj = Cast<IInteractionInterface>(InteractingActor))
	{
		NearestItemInterfaceObj->Execute_EventInteraction(InteractingActor, this);
	}
}

void AItem_PlayableCharacter::Req_PickUp_Implementation(FRotator LookAtRot)
{
	Res_PickUp(LookAtRot);
}

void AItem_PlayableCharacter::Res_PickUp_Implementation(FRotator LookAtRot)
{
	// Set Onwer Character Rotation to Look at this Item
	SetActorRotation(LookAtRot);

	// Play Interaction Montage
	CHECK_VALID(LootingMontage);
	PlayAnimMontage(LootingMontage);
}

void AItem_PlayableCharacter::UseQuickSlot(int32 SlotNum)
{
	QuickSlotComp->UseQuickSlotItem(SlotNum - 1);
}

void AItem_PlayableCharacter::OnWeaponUpdate(UItemData* UpdateEquipItem, const bool& bIsEquip)
{
	FWeaponItemData UpdateWeaponItemData;
	if (!UpdateEquipItem->GetWeaponData(UpdateWeaponItemData))
	{
		return;
	}

	Req_OnWeaponUpdate(UpdateWeaponItemData, bIsEquip);
}

void AItem_PlayableCharacter::Req_OnWeaponUpdate_Implementation(const FWeaponItemData UpdateWeaponItemData, const bool bIsEquip)
{
	Res_OnWeaponUpdate(UpdateWeaponItemData, bIsEquip);
}

void AItem_PlayableCharacter::Res_OnWeaponUpdate_Implementation(const FWeaponItemData UpdateWeaponItemData, const bool bIsEquip)
{
	Weapon->SetEquipMesh(UpdateWeaponItemData.WeaponMesh, bIsEquip);
}

void AItem_PlayableCharacter::OnArmorUpdate(const EArmorType& UpdateArmorType, UItemData* UpdateEquipItem, const bool& bIsEquip)
{
	FArmorItemData UpdateArmorItemData;
	if (!UpdateEquipItem->GetArmorData(UpdateArmorItemData))
	{
		return;
	}

	Req_OnArmorUpdate(UpdateArmorType, UpdateArmorItemData, bIsEquip);
}

void AItem_PlayableCharacter::Req_OnArmorUpdate_Implementation(const EArmorType UpdateArmorType, const FArmorItemData UpdateArmorItemData, const bool bIsEquip)
{
	Res_OnArmorUpdate(UpdateArmorType, UpdateArmorItemData, bIsEquip);
}

void AItem_PlayableCharacter::Res_OnArmorUpdate_Implementation(const EArmorType UpdateArmorType, const FArmorItemData UpdateArmorItemData, const bool bIsEquip)
{
	for (auto ArmorMSMComp : ArmorMSMCompArray)
	{
		if (ArmorMSMComp->GetArmorType() != UpdateArmorType)
		{
			continue;
		}

		ArmorMSMComp->SetEquipMesh(UpdateArmorItemData.ArmorMesh, bIsEquip);

		// if Gloves
		if (UpdateArmorType == EArmorType::Gloves)
		{
			Glove_R->SetEquipMesh(UpdateArmorItemData.AdditionalArmorMesh, bIsEquip);
			return;
		}

		// if Upper - check bShouldSwitchBetweenAdditionalMesh, bIsEquip
		if (UpdateArmorType == EArmorType::Upper)
		{
			Cloak->SetEquipMesh(UpdateArmorItemData.AdditionalArmorMesh, bIsEquip);

			Cloak->SetVisibility(false);
		}

		// if Helmet
		Helmet->SetVisibility(true);
		if (UpdateArmorType == EArmorType::Helmet && UpdateArmorItemData.bHideHairWhenVisible)
		{
			Helmet->SetbHideHairWhenVisible(bIsEquip);
			Hair->SetVisibility(!bIsEquip);
		}
	}
}

void AItem_PlayableCharacter::OnEquipVisibilityUpdate(const EArmorType& UpdateArmorType)
{
	Req_OnEquipVisibilityUpdate(UpdateArmorType);
}

void AItem_PlayableCharacter::Req_OnEquipVisibilityUpdate_Implementation(const EArmorType UpdateArmorType)
{
	Res_OnEquipVisibilityUpdate(UpdateArmorType);
}

void AItem_PlayableCharacter::Res_OnEquipVisibilityUpdate_Implementation(const EArmorType UpdateArmorType)
{
	switch (UpdateArmorType)
	{
	case EArmorType::Helmet:
		Helmet->ToggleVisibility();

		if (Helmet->GetbHideHairWhenVisible())
		{
			Hair->ToggleVisibility();
		}
		return;
	case EArmorType::Upper:
		Cloak->ToggleVisibility();
		return;
	default:
		break;
	}
}
