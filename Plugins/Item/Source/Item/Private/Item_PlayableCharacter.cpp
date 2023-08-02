// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_PlayableCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InteractionInterface.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AItem_PlayableCharacter::AItem_PlayableCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		//Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AItem_PlayableCharacter::Interaction);

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
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);

	double NearestLength = 99999999.0f;
	IInteractionInterface* NearestInteractiveInterfaceObj = nullptr;

	// Find NearestInteractiveInterfaceObj
	for (AActor* Target : OverlappingActors)
	{
		IInteractionInterface* TargetInterfaceObj = Cast<IInteractionInterface>(Target);
		if (!TargetInterfaceObj)
		{
			continue;
		}

		double distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
		if (NearestLength < distance)
		{
			continue;
		}

		NearestLength = distance;

		NearestInteractiveInterfaceObj = TargetInterfaceObj;
		InteractingActor = Target;
	}

	if (!NearestInteractiveInterfaceObj)
	{
		return;
	}

	NearestInteractiveInterfaceObj->Execute_EventInteraction(InteractingActor, this);
}
