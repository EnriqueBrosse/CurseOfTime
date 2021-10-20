// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/Poolable.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class CURSEOFTIME_API AProjectile : public AActor, public IPoolable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// interface functions
	virtual void SetIsActive(bool isActive) override;

	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* Collision;
	UPROPERTY(EditAnywhere)
		float Damage = 10;
	UPROPERTY(EditAnywhere)
		float Velocity = 10000;
	UPROPERTY(EditAnywhere)
		float KillTimer = 10;

	void SetSpawnedFrom(AActor* spawnedFrom);
	void Fire(const FVector& direction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private: 

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void KillSwitch();

	AActor* SpawnedFrom;

	FTimerHandle TimeHandle;

};
