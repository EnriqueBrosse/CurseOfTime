// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../Interfaces/StatusEffectInterface.h"
#include "../../Interfaces/Poolable.h"
#include "../../Actors/MeleeWeapon.h" // change this to only include the struct where needed 
#include "BaseEnemy.generated.h"

class UHealthComponent;
class UBoxComponent;
class UPawnSensingComponent;
class AEnemyManager;
class UNiagaraComponent;

UCLASS()
class CURSEOFTIME_API ABaseEnemy : public ACharacter, public IStatusEffectInterface, public IPoolable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();
	//  interface functions 
	void OnDeath() override;
	void OnStun() override;
	void OnKnockBack(const FVector& knockbackValue) override;
	void OnHit() override;
	void OnShieldHit() override;

	void SetIsActive(bool isActive)override;

	AEnemyManager* GetEnemyManager()const { return EnemyManager; }
	void SetEnemyManager(AEnemyManager* enemyManager) { EnemyManager = enemyManager; }

	bool GetIsDead()const; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* SpawnNiagaraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* DespawnNiagaraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float DespawnNiagaraDelayFromAnim = 1;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int ScoreOnKill;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundAttenuation* AttenuationSettings;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundConcurrency* ConcurrencySettings;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* StunMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PlayerFeedback")
		UAnimMontage* hitMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float CutoffDeathMontage = 0.2f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	AEnemyManager* EnemyManager; 

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UPawnSensingComponent* PawnSensingComponent;
private:

	UPROPERTY()
		FTimerHandle DeathTimeHandle;
	ACharacter* Player;

	UFUNCTION()
		void Reset();
	UFUNCTION()
		void OnSeePawn(APawn* seenPawn);

	UFUNCTION()
		void SpawnDeaspawnNiagara();
};
