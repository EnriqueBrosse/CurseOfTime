// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpawnerComponent.generated.h"

class ABaseEnemy;
class AEnemyManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTIME_API USpawnerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnerComponent();

	void StartSpawning(bool canSpawn);
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy(); 
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	AEnemyManager* EnemyManager;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABaseEnemy>  EnemyType;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float SpawnInterval;
	UPROPERTY()
		FTimerHandle TimeHandle;
};
