// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DashComponent.generated.h"

class UStaminaComponent;
class UHealthComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTIME_API UDashComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDashComponent();
	void Dash();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int MaxAmountOfDashes;
	int CurrentAmountOfDashes; 

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float DashDelay;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float DashVelocity;

	UStaminaComponent* StaminaComponent;
	UHealthComponent* HealthComponent;
	UFUNCTION()
		void AddDash();
};
