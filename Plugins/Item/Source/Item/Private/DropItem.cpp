// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItem.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ADropItem::ADropItem()
{
	RootSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = RootSphereCollision;
	RootSphereCollision->SetSimulatePhysics(true);
	RootSphereCollision->SetNotifyRigidBodyCollision(true);
	//RootSphereCollision->SetCollisionProfileName(FName("DropItem"));
	RootSphereCollision->OnComponentHit.AddDynamic(this, &ADropItem::OnHit);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetCollisionProfileName(FName("NoCollision"));

	OverlapBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBoxCollision"));
	OverlapBoxCollision->SetupAttachment(RootComponent);
	OverlapBoxCollision->SetCollisionProfileName(FName("OverlapAllDynamic"));

}

// Called when the game starts or when spawned
void ADropItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADropItem::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	RootSphereCollision->SetSimulatePhysics(false);
	RootSphereCollision->SetNotifyRigidBodyCollision(false);
}

void ADropItem::EventLoot_Implementation(ACharacter* OwnCharacter)
{
	// Set Onwer Character Rotation to Look at this Item
	FVector TargetDir = (GetActorLocation() - OwnCharacter->GetActorLocation()).GetSafeNormal();
	FRotator LookAtRot = FVector(TargetDir.X, TargetDir.Y, 0).Rotation();
	OwnCharacter->SetActorRotation(LookAtRot);

	// Play Interaction Montage
	ensureMsgf(IsValid(LootingMontage), TEXT("LootingMontage is not valid"));
	OwnCharacter->PlayAnimMontage(LootingMontage);
}
