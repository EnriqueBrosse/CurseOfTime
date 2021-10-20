// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ghost.generated.h"

class UGhostComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
UCLASS()
class CURSEOFTIME_API AGhost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhost();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* Capsule;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UGhostComponent* GhostComponent;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Mesh;
};
