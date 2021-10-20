// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerComponent.h"
#include "../Actors/EnemyManager.h"
#include "../Characters/Enemies/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USpawnerComponent::USpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void USpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> managers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyManager::StaticClass(), managers);
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

}

void USpawnerComponent::SpawnEnemy()
{
	if (GetWorld())
	{
		ABaseEnemy* enemy = EnemyManager->GetEnemy(EnemyType);
		enemy->SetActorLocationAndRotation(GetComponentLocation(),GetComponentRotation());
	}
}
