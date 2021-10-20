// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeWeapon.h" // remove this in the next sprint with a header for the struct
#include "EnemyManager.generated.h"

class ABaseEnemy;
class AFighterEnemy;
class UKungFuGrid;
struct FAttacks;

UCLASS()
class CURSEOFTIME_API AEnemyManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	ABaseEnemy* GetEnemy(TSubclassOf<ABaseEnemy> enemyType);
	bool RequestAccesToApproachCircle(AFighterEnemy* enemy);

	bool RequestAttack(AFighterEnemy* enemy, FAttacks& returnAttack);

	bool GetPositionFromNodeIndex(int nodeIndex, FVector& position);
	void RemoveEnemyFromCircle(AFighterEnemy* enemy);
	void ChooseLocationOutsideCircle(ABaseEnemy* enemy, FVector& location);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private: 

	TMap<TSubclassOf<ABaseEnemy>, TArray<ABaseEnemy*>> EnemyPool;


	int CurrentGridCapacity = 0; 
	int CurrentAttackCapacity = 0;
	TArray<ABaseEnemy*> EnemiesInApproachCircle;
	TMap<ABaseEnemy*,FAttacks> CurrentAttacks;

	ACharacter* Player;
	UKungFuGrid* KungFuGrid;
};
