// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileManager.generated.h"

class AProjectile;
UCLASS()
class CURSEOFTIME_API AProjectileManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileManager();
	// Called every frame
	AProjectile* GetProjectile(TSubclassOf<AProjectile> projectileType);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private: 
	TMap<TSubclassOf<AProjectile>, TArray<AProjectile*>> ProjectilePool;

};
