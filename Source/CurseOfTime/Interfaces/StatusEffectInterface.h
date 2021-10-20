// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatusEffectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStatusEffectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CURSEOFTIME_API IStatusEffectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void OnDeath() = 0; 
	virtual void OnHit() = 0; 
	virtual void OnStun() = 0;
	virtual void OnShieldHit() = 0;
	virtual void OnKnockBack(const FVector& knockbackValue) = 0;
};
