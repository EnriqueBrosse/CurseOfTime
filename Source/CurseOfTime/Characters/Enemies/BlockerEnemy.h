// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FighterEnemy.h"
#include "BlockerEnemy.generated.h"

class AShield;
UCLASS()
class CURSEOFTIME_API ABlockerEnemy : public AFighterEnemy
{
	GENERATED_BODY()
public: 
	ABlockerEnemy();
	void OnShieldHit() override;

	void SetIsActive(bool isActive)override;
protected: 
	virtual void BeginPlay() override;

private: 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AShield>  ShieldToSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FName SocketShield = "";

		AShield* Shield;

		UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
			USoundBase* ShieldHitSound;

		UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Player feedback")
			UAnimMontage* ShieldHit;
};
