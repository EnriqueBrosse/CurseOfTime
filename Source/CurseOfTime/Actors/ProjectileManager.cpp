// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileManager.h"
#include "Projectile.h"

// Sets default values
AProjectileManager::AProjectileManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AProjectileManager::BeginPlay()
{
	Super::BeginPlay();
	
}

AProjectile* AProjectileManager::GetProjectile(TSubclassOf<AProjectile> projectileType)
{
	if (!ProjectilePool.Contains(projectileType))
	{
		ProjectilePool.Add(projectileType);
	}

	for (AProjectile* projectile : ProjectilePool[projectileType])
	{
		// the projectile has to have hit anything/had its kill timer gone off
		if (!projectile->GetIsActive() && !projectile->GetIsOccupied())
		{
			projectile->SetIsOccupied(true);
			projectile->SetIsActive(true);
			return projectile;
		}
	}

	// if there are no enemies on non active and not occupied make a new one and send that one 
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileType, spawnParameters);
	projectile->SetIsOccupied(true);
	projectile->SetIsActive(true);
	ProjectilePool[projectileType].Add(projectile);

	return projectile;
}
