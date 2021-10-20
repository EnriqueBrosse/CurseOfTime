// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"
// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentAmountOfStamina = MaxAmountOfStamina;
}

void UStaminaComponent::UseStamina(int amount)
{
	if (CurrentAmountOfStamina - amount < 0)
	{
		UE_LOG(LogTemp,Error,TEXT("UStaminaComponent::UseStamina something went wrong, not enough stamina"))
		return;
	}
	CurrentAmountOfStamina -= amount;
	for (int i = 0; i < amount; i++)
	{
		FTimerHandle UniqueHandle;
		float rand = 1.f + (float(FMath::RandRange(-1.f, 1.f)) * StaminaRegenRandMultiplier);
		if (rand <= 0.f) //safety check
		{
			rand = 0.1f;
		}
		GetWorld()->GetTimerManager().SetTimer(UniqueHandle, this, &UStaminaComponent::IncreaseStamina, StaminaRegenDelay * rand, false);
	}
}

void UStaminaComponent::IncreaseStamina()
{
	CurrentAmountOfStamina++;
}