// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADropItem::ADropItem()
{
	RootSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = RootSphereCollision;
	RootSphereCollision->SetSimulatePhysics(true);
	RootSphereCollision->SetGenerateOverlapEvents(false);
	RootSphereCollision->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	RootSphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ItemParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ItemParticle"));
	ItemParticle->SetupAttachment(RootComponent);

	OverlapBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBoxCollision"));
	OverlapBoxCollision->SetupAttachment(RootComponent);
	OverlapBoxCollision->SetCollisionProfileName(FName("OverlapAllDynamic"));

	LootWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("LootWidget"));
	LootWidgetComp->SetupAttachment(RootComponent);
	LootWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	LootWidgetComp->SetDrawSize(FVector2D(300.f, 100.f));
	LootWidgetComp->SetVisibility(false);

}

// Called when the game starts or when spawned
void ADropItem::BeginPlay()
{
	Super::BeginPlay();

	ensureMsgf(LootWidgetComp->GetWidgetClass(), TEXT("Null in Widget Component."));

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ensureMsgf(PlayerController, TEXT("PlayerController is nullptr"));

	GetWorldTimerManager().SetTimer(UpdateWidgetVisiblityTimerHandle, this, &ADropItem::UpdateWidgetVisiblity, 0.1f, true);
}

void ADropItem::UpdateWidgetVisiblity()
{
	if (IsCanSeePlayer())
	{
		if (!bWidgetVisibility)
		{
			LootWidgetComp->SetVisibility(true);
			bWidgetVisibility = true;
		}
	}
	else
	{
		if (bWidgetVisibility)
		{
			LootWidgetComp->SetVisibility(false);
			bWidgetVisibility = false;
		}
	}
}

bool ADropItem::IsCanSeePlayer()
{
	FHitResult Hit;

	FVector StartPos = PlayerController->PlayerCameraManager->GetCameraLocation();
	// FVector EndPos = GetActorLocation();
	FVector EndPos = StartPos + (GetActorLocation() - StartPos).GetSafeNormal() * WidgetVisiblityRange;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerController->GetPawn());

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos, ECC_Visibility, CollisionParams);

	if (bIsHit)
	{
		if (Hit.GetActor() == this)
		{
			return true;
		}
	}

	return false;
}

void ADropItem::EventLoot_Implementation(ACharacter* OwnCharacter)
{
	// Hide UI Widget
	LootWidgetComp->SetHiddenInGame(true);

	// Set Onwer Character Rotation to Look at this Item
	FVector TargetDir = (GetActorLocation() - OwnCharacter->GetActorLocation()).GetSafeNormal();
	FRotator LookAtRot = FVector(TargetDir.X, TargetDir.Y, 0).Rotation();
	OwnCharacter->SetActorRotation(LookAtRot);

	// Play Interaction Montage
	ensureMsgf(IsValid(LootingMontage), TEXT("LootingMontage is not valid"));
	OwnCharacter->PlayAnimMontage(LootingMontage);
}
