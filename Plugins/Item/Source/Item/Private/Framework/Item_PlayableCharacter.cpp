// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_PlayableCharacter.h"
//Temp
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Item.h"
//Enhanced Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//Interfaces
#include "InteractionInterface.h"
//Components
#include "Components/CapsuleComponent.h"

#include "Item_FHPlayerController.h"
#include "QuickSlotComponent.h"

// Sets default values
AItem_PlayableCharacter::AItem_PlayableCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	// Interact Collision
	InteractCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->SetCapsuleSize(60.f, 120.f);

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
}

// Called every frame
void AItem_PlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	if (!QuickSlotComp)
	{
		AItem_FHPlayerController* PC = Cast<AItem_FHPlayerController>(GetController());
		CHECK_VALID(PC);

		QuickSlotComp = PC->GetQuickSlotComp();
	}
	CHECK_VALID(QuickSlotComp);
	
	QuickSlotComp->UseQuickSlotItem(SlotNum - 1);
}
