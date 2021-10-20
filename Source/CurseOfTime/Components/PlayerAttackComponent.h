// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackComponent.generated.h"

class UHealthComponent; 
class UStaminaComponent;
class AMeleeWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTIME_API UPlayerAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAttackComponent();

	UFUNCTION(BlueprintCallable)
		void OnLightAttack();

	UFUNCTION(BlueprintCallable)
		void OnHeavyAttack();

	UFUNCTION(BlueprintCallable)
		void DoAttack();

	UFUNCTION(BlueprintCallable)
	void SetCurrentWeapon(AMeleeWeapon* weapon);

	UFUNCTION(BlueprintCallable)
		void ComboAttack();

	UFUNCTION(BlueprintCallable)
		void ResetCombo();
	UFUNCTION(BlueprintCallable)
		AMeleeWeapon* GetCurrentWeapon()const { return CurrentMeleeWeapon; }

	bool GetIsAttacking()const { return bIsAttacking; }

	void SetIsAttacking(bool isAttacking) { bIsAttacking = isAttacking; }

	void SetNormalizedMultiplier(float multiplier) { NormalizedMultiplier = multiplier; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	

	enum class AttackInput
	{
		LightAttack, HeavyAttack, None
	};

	UPROPERTY(EditAnywhere, Category = KillingSpree, meta = (AllowPrivateAccess = "true"))
		float MaxAmountOfAddedSpeed = 300;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float NormalizedSlowDownMovementspeedAttacking = 0.5f;

	UPROPERTY(EditAnywhere, Category = KillingSpree, meta = (AllowPrivateAccess = "true"))
		float MaxAmountOfAddedAttackingSpeed = 1;

	float NormalizedMultiplier; // value between 0-1

	float NormalMovementSpeed;

	bool bResetCombo;
	bool bIsAttacking;
	bool bSaveAttack;
	int AttackNumber = 0;
	AttackInput AttackInput = AttackInput::None;
	AMeleeWeapon* CurrentMeleeWeapon = nullptr;

	UHealthComponent* HealthComponent;
	UStaminaComponent* StaminaComponent;
	ACharacter* Player = nullptr;
};
