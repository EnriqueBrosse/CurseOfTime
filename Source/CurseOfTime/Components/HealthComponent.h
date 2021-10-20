// Fill out your copyright notice in the Description page of Project Settings.
// Made by Enrique Brosse
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"
class IStatusEffectInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTIME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	void DoDamage(float damage, bool playerFeedback = false);
	void Heal(float health);
	void Reset();

	bool GetIsDead()const { return bIsDead; }
	float GetCurrentHealth()const { return CurrentHealth; }
	float GetMaxHealth()const { return MaxHealth; }
	void SetHealth(float health);
	void SetMaxHealth(float health);
	void SetIsInvonerable(bool isInvonarable){ bIsInonerable = isInvonarable; }
	void SetLooseHealthEverySecond(bool looseHealthEverySecond){ bLooseHealthEverySecond = looseHealthEverySecond; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health , meta = (AllowPrivateAccess = "true"))
		float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = Health , meta = (AllowPrivateAccess = "true"))
		bool bCanRegenHealth = false;

	UPROPERTY(EditAnywhere, Category = Health , meta = (AllowPrivateAccess = "true"))
		float RegenHealthAmountPerSecond;

	// initial time in seconds until it will regen health
	UPROPERTY(EditAnywhere, Category = Health , meta = (AllowPrivateAccess = "true"))
		float MaxRegenTime = 1;

	// the time from when it starts regening to the next regen
	UPROPERTY(EditAnywhere, Category = Health , meta = (AllowPrivateAccess = "true"))
		float TimeInBetweenRegening = 1;

	UPROPERTY(EditAnywhere, Category = Health, meta = (AllowPrivateAccess = "true"))
		bool bIsInonerable;

	UPROPERTY(EditAnywhere, Category = Health, meta = (AllowPrivateAccess = "true"))
		bool bLooseHealthEverySecond = false;

	float TimeUntilRegeningHealth;
	UPROPERTY(EditAnywhere, Category = Health, meta = (AllowPrivateAccess = "true"))
		bool bIsDead = false;
	IStatusEffectInterface* StatusEffectInterface;
};
