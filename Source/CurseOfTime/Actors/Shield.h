// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"
class UStaticMeshComponent;
class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class CURSEOFTIME_API AShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShield();

	void TurnOnCollision();
	void TurnOffCollision();

	void TurnOnNiagara(); 
protected:
	//// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private: 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* NiagaraComp; 
};
