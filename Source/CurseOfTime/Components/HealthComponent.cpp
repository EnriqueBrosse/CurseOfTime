// Fill out your copyright notice in the Description page of Project Settings.
// Made by Enrique Brosse

#include "HealthComponent.h"
#include "../Interfaces/StatusEffectInterface.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	StatusEffectInterface = Cast<IStatusEffectInterface>(GetOwner());
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsDead || bIsInonerable)
	{
		return;
	}
	if (bCanRegenHealth && !bIsDead && CurrentHealth != MaxHealth)
	{
		TimeUntilRegeningHealth -= DeltaTime;
		if (TimeUntilRegeningHealth < 0)
		{
			TimeUntilRegeningHealth += TimeInBetweenRegening;
			Heal(RegenHealthAmountPerSecond);
		}
	}
	if (bLooseHealthEverySecond)
	{
		DoDamage(DeltaTime);

	}
}

void UHealthComponent::SetHealth(float health)
{
	CurrentHealth = health;
	bIsDead = CurrentHealth <= 0;
}
void UHealthComponent::SetMaxHealth(float health)
{
	MaxHealth = health;
}

void UHealthComponent::DoDamage(float damage, bool playerFeedback)
{
	if (bIsDead || bIsInonerable)
	{
		return;
	}

	// making sure nobody wants to heal - damage 
	damage = std::abs(damage); 
	CurrentHealth -= damage; 
	if (bCanRegenHealth)
	{
		TimeUntilRegeningHealth = MaxRegenTime;
	}

	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		bIsDead = true;
		// add the interface stuff here
		IStatusEffectInterface* statusEffectInterfacle = Cast<IStatusEffectInterface>(GetOwner());
		if (statusEffectInterfacle != nullptr)
		{
			statusEffectInterfacle->OnDeath();			
		}
	}
	else if (playerFeedback && StatusEffectInterface)
	{
		StatusEffectInterface->OnHit();
	}
}

void UHealthComponent::Heal(float health)
{
	if (bIsDead)
	{
		UE_LOG(LogTemp, Error, TEXT("UHealthComponent::Heal %s is dead "), *GetOwner()->GetName())
	}
	// making sure nobody wants to heal - health 
	health = std::abs(health);

	CurrentHealth += health;

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}

void UHealthComponent::Reset()
{
	CurrentHealth = MaxHealth;
	bIsDead = false;
}
