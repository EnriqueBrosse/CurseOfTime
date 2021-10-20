// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTIME_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	UFUNCTION()
	void UseStamina(int amount = 1);
	UFUNCTION(BlueprintCallable)
	int GetCurrentAmountOfStamina()const { return CurrentAmountOfStamina; }
	UFUNCTION(BlueprintCallable)
		int GetMaxAmountOfStamina()const { return MaxAmountOfStamina; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float StaminaRegenDelay = 1;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float StaminaRegenRandMultiplier = 0.5;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int MaxAmountOfStamina = 5;
	int CurrentAmountOfStamina;
		
	UFUNCTION()
		void IncreaseStamina(); 
};
