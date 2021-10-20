// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../Components/HealthComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(Collision);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::SetIsActive(bool isActive)
{
	bIsActive = isActive;
	SetActorHiddenInGame(!bIsActive);

	if (isActive)
	{
		GetWorldTimerManager().SetTimer(TimeHandle, this, &AProjectile::KillSwitch, KillTimer, false);
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimeHandle);
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ProjectileMovementComponent->Velocity = FVector();
	}
}

void AProjectile::SetSpawnedFrom(AActor* spawnedFrom)
{
	if (spawnedFrom->IsValidLowLevelFast())
	{
		SpawnedFrom = spawnedFrom;
	}
}

void AProjectile::Fire(const FVector& direction)
{
	FVector velocity = direction * Velocity;
	ProjectileMovementComponent->Velocity = velocity;
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if it hits the itself or it hits the actor it was from it will not despawn 
	if (OtherActor == this || SpawnedFrom == OtherActor)
	{
		return;
	}
	SetIsActive(false);
	SetIsOccupied(false);
	UHealthComponent* healthComp = OtherActor->FindComponentByClass<UHealthComponent>();
	UE_LOG(LogTemp, Warning, TEXT("%s overlapped with %s"), *GetName(), *OtherActor->GetName());

	if (healthComp)
	{
		healthComp->DoDamage(Damage, true);
	}
}

void AProjectile::KillSwitch()
{
	if (bIsActive)
	{
		SetIsActive(false);
		SetIsOccupied(false);
	}
}
