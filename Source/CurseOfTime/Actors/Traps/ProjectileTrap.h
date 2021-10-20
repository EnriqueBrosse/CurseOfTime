// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTrap.h"
#include "ProjectileTrap.generated.h"

class UStaticMeshComponent;
class AProjectile;
class UArrowComponent;
class AProjectileManager;

UCLASS()
class CURSEOFTIME_API AProjectileTrap : public ABaseTrap
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileTrap();

	virtual void Execute()override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* ShootingMesh;

	// this is used for the direction and the position of where the projectile is going to shoot
	UPROPERTY(EditAnywhere)
		UArrowComponent* ArrowComp;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile>  Projectile;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	AProjectileManager* ProjectileManager;
};
