// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_PlayableCharacter.h"
#include "Item.h"
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

// Sets default values
AItem_PlayableCharacter::AItem_PlayableCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass(ACharacter::MeshComponentName, UModularSkeletalMeshComponent::StaticClass()))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 100.f);

	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Set Modular Meshes
	if (UModularSkeletalMeshComponent* LowerBody = Cast<UModularSkeletalMeshComponent>(GetMesh()))
	{
		LowerBody->SetArmorType(EArmorType::Lower);
		ArmorMSMCompArray.Add(LowerBody);
	}

	Shoes = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Shoes"));
	InitModularMeshComp(Shoes, EArmorType::Shoes, true);

	UpperBody = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("UpperBody"));
	InitModularMeshComp(UpperBody, EArmorType::Upper, true);

	Cloak = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Cloak"));
	InitModularMeshComp(Cloak, EArmorType::None, true);

	Glove_L = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Glove_L"));
	InitModularMeshComp(Glove_L, EArmorType::Gloves, true);

	Glove_R = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Glove_R"));
	InitModularMeshComp(Glove_R, EArmorType::None, true);

	Head = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Head"));
	InitModularMeshComp(Head, EArmorType::None, true);

	Hair = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Hair"));
	InitModularMeshComp(Hair, EArmorType::None, true);

	Helmet = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Helmet"));
	InitModularMeshComp(Helmet, EArmorType::Helmet, false);

	Weapon = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Weapon"));
	InitModularMeshComp(Weapon, EArmorType::None, false);

	// Interact Collision
	InteractCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->SetCapsuleSize(60.f, 120.f);
}

void AItem_PlayableCharacter::InitModularMeshComp(UModularSkeletalMeshComponent* ModularMeshComp, const EArmorType& NewArmorType, const bool& bSetLeaderPoseComp)
{
	ModularMeshComp->SetupAttachment(GetMesh());
	ModularMeshComp->SetArmorType(NewArmorType);

	if (NewArmorType != EArmorType::None)
	{
		ArmorMSMCompArray.Add(ModularMeshComp);
	}

	if (bSetLeaderPoseComp)
	{
		ModularMeshComp->SetLeaderPoseComponent(GetMesh());
	}
}

// Called when the game starts or when spawned
void AItem_PlayableCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	PC = Cast<AItem_FHPlayerController>(GetController());
	CHECK_VALID(PC);

	QuickSlotComp = PC->GetQuickSlotComp();
	CHECK_VALID(QuickSlotComp);

	UEquipmentComponent* EquipmentComp = PC->GetEquipmentComp();
	CHECK_VALID(EquipmentComp);

	// Bind Equipment Update Delegates
	EquipmentComp->WeaponUpdateDelegate.AddUObject(this, &AItem_PlayableCharacter::OnWeaponUpdate);
	EquipmentComp->ArmorUpdateDelegate.AddUObject(this, &AItem_PlayableCharacter::OnArmorUpdate);

	CloakUpdateDelegate.AddDynamic(this, &AItem_PlayableCharacter::OnCloakUpdate);
}

// Called to bind functionality to input
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

	return;
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

	Weapon->SetEquipMesh(UpdateWeaponItemData.WeaponMesh, bIsEquip);
}

void AItem_PlayableCharacter::OnArmorUpdate(const EArmorType& UpdateArmorType, UItemData* UpdateEquipItem, const bool& bIsEquip)
{
	FArmorItemData UpdateArmorItemData;
	if (!UpdateEquipItem->GetArmorData(UpdateArmorItemData))
	{
		return;
	}

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

		// if Upper
		if (UpdateArmorType == EArmorType::Upper)
		{
			Cloak->SetVisibility(true);

			if (UpdateArmorItemData.bShouldSwitchBetweenAdditionalMesh)
			{
				Cloak->SetEquipMesh(UpdateArmorItemData.AdditionalArmorMesh, !bIsEquip);
				return;
			}

			Cloak->SetEquipMesh(UpdateArmorItemData.AdditionalArmorMesh, bIsEquip);
			return;
		}
	}
}

void AItem_PlayableCharacter::OnCloakUpdate(bool bVisibility)
{
	if (!Cloak->GetSkeletalMeshAsset())
	{
		return;
	}

	Cloak->SetVisibility(bVisibility);
}
