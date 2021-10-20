// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileTrap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../ProjectileManager.h"
#include "../Projectile.h"
#include "NiagaraComponent.h"

// Sets default values
AProjectileTrap::AProjectileTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ShootingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShootingMesh"));
	SetRootComponent(ShootingMesh);
	ArrowComp->SetupAttachment(ShootingMesh);
	NiagaraComponent->SetupAttachment(RootComponent);
}

void AProjectileTrap::Execute()
{
	Super::Execute();
	AProjectile* projectile = ProjectileManager->GetProjectile(Projectile);
	projectile->SetSpawnedFrom(this);
	projectile->SetActorLocationAndRotation(ArrowComp->GetComponentLocation(), ArrowComp->GetComponentRotation());
	projectile->Fire(ArrowComp->GetForwardVector());
}

// Called when the game starts or when spawned
void AProjectileTrap::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> managers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProjectileManager::StaticClass(), managers);
	// if there are none, spawn them 
	if (managers.Num() == 0)
	{
		FActorSpawnParameters spawnParameters;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ProjectileManager = GetWorld()->SpawnActor<AProjectileManager>(spawnParameters);
	}
	else
	{
		ProjectileManager = Cast<AProjectileManager>(managers[0]);
	}
}
