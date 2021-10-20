// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTrap.h"
#include "Components/TimelineComponent.h"
#include "SpikeTrap.generated.h"

class UStaticMeshComponent;
class UCurveFloat;

UCLASS()
class CURSEOFTIME_API ASpikeTrap : public ABaseTrap
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpikeTrap();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Execute()override; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private: 
	UPROPERTY(EditAnywhere, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
		UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline" , meta = (AllowPrivateAccess = "true"))
		float SpikeUpOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* SpikeMesh;

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		int Damage = 10;

		FVector StartLocation;
		FVector EndLocation;
		FTimeline TimeLine;
	
	UFUNCTION()
		void OnOverlapBeginSpikes(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void TimelineProgress(float value);
};
