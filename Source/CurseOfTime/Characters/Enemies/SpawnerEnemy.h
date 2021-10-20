// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "SpawnerEnemy.generated.h"

class USpawnerComponent;
UCLASS()
class CURSEOFTIME_API ASpawnerEnemy : public ABaseEnemy
{
	GENERATED_BODY()
public: 
	ASpawnerEnemy();
	FORCEINLINE UAnimMontage* GetSpawnMontage()const { return AnimationToPlayForSpawning; }
	FORCEINLINE float GetAnimPlayRate()const { return AnimPlayRate; }
protected:

private: 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		USpawnerComponent* SpawnerComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AnimationToPlayForSpawning;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float AnimPlayRate = 1;
};
