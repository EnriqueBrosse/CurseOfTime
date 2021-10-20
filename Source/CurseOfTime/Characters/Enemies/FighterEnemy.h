// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "FighterEnemy.generated.h"

class AMeleeWeapon;
struct FAttacks;

UCLASS()
class CURSEOFTIME_API AFighterEnemy : public ABaseEnemy
{
	GENERATED_BODY()
public: 
	void SetCurrentAttack(const FAttacks& currentAttack);
	FORCEINLINE const FAttacks& GetCurrentAttack()const { return MeleeWeapon->GetCurrentAttack(); }
	int GetGridWeight()const { return GridWeight; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMeleeWeapon* GetMeleeWeapon()const { return MeleeWeapon; }

	void SetIsActive(bool isActive)override;

	void OnHit() override;
	void OnDeath() override;

protected: 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = KungFuCircle, meta = (AllowPrivateAccess = "true"))
		int GridWeight;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMeleeWeapon>  WeaponToSpawnOnBeginPlay;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FName SocketWeapon = "";

	AMeleeWeapon* MeleeWeapon;
};
