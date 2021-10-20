// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pedestal.generated.h"

class AMeleeWeapon; 
class UStaticMeshComponent;
class UBoxComponent;
class UArrowComponent;
class APlayerCharacter;
UCLASS()
class CURSEOFTIME_API APedestal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APedestal();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private: 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMeleeWeapon> WeaponToSpawn; 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UArrowComponent* WeaponSpawn;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMesh; 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComp; 

	AMeleeWeapon* MeleeWeapon;
	APlayerCharacter* Player;
	UFUNCTION()
	void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
