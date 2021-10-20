// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class ABaseEnemy;
class UBoxComponent;
class AEnemyManager;

USTRUCT(BlueprintType)
struct FWave
{
	GENERATED_USTRUCT_BODY();
public:
	// the enemy class to spawn
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABaseEnemy>  EnemyType;

	// the amount of waves/enemies this one wave will spawn
	UPROPERTY(EditAnywhere)
		int AmountOfEnemies = 10;

	// time inbetween the last enemy spawn
	UPROPERTY(EditAnywhere)
		float TimeUntilNextSpawn = 1.f;

	bool bIsDoneSpawning = false;
	bool bCanGoToNextWave = false;
};


UCLASS()
class CURSEOFTIME_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartSpawning(bool startNow = false);

	bool GetIsDoneSpawning()const { return bIsDoneSpawning; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private: 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TArray<FWave> Waves;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool bIsDoneSpawning = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		bool bSpawnOnBeginPlay = false;

	UPROPERTY()
		FTimerHandle TimeHandle;

	UPROPERTY()
		AEnemyManager* EnemyManager;
	
	UPROPERTY()
		TArray<ABaseEnemy*> SpawnedEnemies;
	UFUNCTION()
		void Spawn();

};
