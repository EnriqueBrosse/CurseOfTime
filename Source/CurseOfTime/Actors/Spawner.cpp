// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "EnemyManager.h"
#include "../Characters/Enemies/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	// looking if there are enemy managers in the world 
	TArray<AActor*> managers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyManager::StaticClass(), managers);
	// if there are none, spawn them 
	if (managers.Num() == 0)
	{
		FActorSpawnParameters spawnParameters;
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		EnemyManager = GetWorld()->SpawnActor<AEnemyManager>(spawnParameters);
	}
	else
	{
		EnemyManager = Cast<AEnemyManager>(managers[0]);
	}

	if (bSpawnOnBeginPlay && Waves.IsValidIndex(0))
	{
		// setting the timer to release Enemies
		GetWorldTimerManager().SetTimer(TimeHandle, this, &ASpawner::Spawn, Waves[0].TimeUntilNextSpawn, true);
	}
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawner::Spawn()
{
	for (size_t i = 0; i < Waves.Num(); i++)
	{
		if (Waves[i].bIsDoneSpawning)
		{
			if (Waves[i].bCanGoToNextWave)
			{
				continue;
			}
			// checking if enemies are dead
			for (ABaseEnemy* enemy : SpawnedEnemies)
			{
				// if one of them is active it means that they're not dead yet
				if (!enemy->GetIsDead())
				{
					return;
				}
			}
			// if they're all dead, set them all to non occupied this will make it so they're "send back to the manager"
			for (ABaseEnemy* enemy : SpawnedEnemies)
			{
				enemy->SetIsOccupied(false);
			}
			// clearing the array
			SpawnedEnemies.SetNum(0);
			Waves[i].bCanGoToNextWave = true;
			if ((i + 1) == Waves.Num())
			{
				if (GetWorld())
				{
					FTimerManager& timemanager = GetWorldTimerManager();
					if (timemanager.IsTimerActive(TimeHandle))
					{
						timemanager.ClearTimer(TimeHandle);
					}
				}
				bIsDoneSpawning = true;
				// clearing the timer and setting a new one 
				return;
			}
			GetWorldTimerManager().SetTimer(TimeHandle, this, &ASpawner::Spawn, Waves[i + 1].TimeUntilNextSpawn, true);
		}
		if (Waves[i].EnemyType == NULL)
		{
			UE_LOG(LogTemp,Error,TEXT("ASpawner::Spawn Enemy Type is NULL"))
			return;
		}
		ABaseEnemy* enemy =	EnemyManager->GetEnemy(Waves[i].EnemyType);
		FVector pos = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), BoxComponent->GetScaledBoxExtent());
		pos.Z = GetActorLocation().Z;
		enemy->SetActorLocationAndRotation(pos, GetActorRotation());

		// adding the enemy to the vector This can go wrong if multiple spawners will ask for the same enemy
		SpawnedEnemies.Add(enemy);

		UE_LOG(LogTemp, Warning, TEXT("Enemy Spawn"));
		UE_LOG(LogTemp, Warning, TEXT("%i"), Waves[i].AmountOfEnemies);
		Waves[i].AmountOfEnemies--;
		if (Waves[i].AmountOfEnemies == 0)
		{
			Waves[i].bIsDoneSpawning = true;
		}
		return;
	}
}

void ASpawner::StartSpawning(bool startNow)
{
	if (Waves.IsValidIndex(0))
	{
		// setting the timer to release Enemies
		GetWorldTimerManager().SetTimer(TimeHandle, this, &ASpawner::Spawn, Waves[0].TimeUntilNextSpawn, true);
	}
	if (startNow)
	{
		Spawn();
	}
}
