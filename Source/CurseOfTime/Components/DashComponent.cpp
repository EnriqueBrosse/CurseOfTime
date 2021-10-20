// Fill out your copyright notice in the Description page of Project Settings.


#include "DashComponent.h"
#include "StaminaComponent.h"
#include "HealthComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UDashComponent::UDashComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
void UDashComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmountOfDashes = MaxAmountOfDashes;
	StaminaComponent = GetOwner()->FindComponentByClass<UStaminaComponent>();
	HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
}

void UDashComponent::Dash()
{
	if (HealthComponent->GetIsDead())
	{
		return;
	}

	if (CurrentAmountOfDashes == 0)
	{
		return;
	}
	// doens't matter if it has it here or not but if it has a stamina component we're going to do an extra check
	if (StaminaComponent->IsValidLowLevel())
	{
		if (StaminaComponent->GetCurrentAmountOfStamina() <= 0)
		{
			return;
		}
		StaminaComponent->UseStamina();
	}
	ACharacter* character = Cast<ACharacter>(GetOwner());
	if (!character->IsValidLowLevelFast())
	{
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("UDashComponent::Dash currentAmountOfDashes %i"), CurrentAmountOfDashes)

		CurrentAmountOfDashes--;
	character->LaunchCharacter(character->GetActorForwardVector() * DashVelocity, true, false);
	FTimerHandle UniqueHandle;
	GetWorld()->GetTimerManager().SetTimer(UniqueHandle, this, &UDashComponent::AddDash, DashDelay, false);
}
void UDashComponent::AddDash()
{
	CurrentAmountOfDashes++;
	UE_LOG(LogTemp, Error, TEXT("UDashComponent::AddDash CurrentAmountOfDashes %i"), CurrentAmountOfDashes)

		return;
}
