// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTrap.generated.h"

class UNiagaraComponent;

UCLASS(Abstract)
class CURSEOFTIME_API ABaseTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseTrap();
	virtual void Execute();
protected: 
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* NiagaraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float NiagaraSpawnDelay = 1.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundAttenuation* AttenuationSettings;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundConcurrency* ConcurrencySettings;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundBase* TrapSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float SoundDelay = 1.f;

	FTimerHandle NiagaraTimeHandle;
	FTimerHandle SoundTimeHandle;

	UFUNCTION()
		void ExecuteNiagara();

	UFUNCTION()
		void ExecuteSound();
};
