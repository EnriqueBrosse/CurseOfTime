// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttackComponent.h"
#include "HealthComponent.h"
#include "StaminaComponent.h"
#include "../Actors/MeleeWeapon.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UPlayerAttackComponent::UPlayerAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ACharacter>(GetOwner());
	HealthComponent = Player->FindComponentByClass<UHealthComponent>();
	StaminaComponent = Player->FindComponentByClass<UStaminaComponent>();
	NormalMovementSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;
	// ...
}

void UPlayerAttackComponent::OnLightAttack()
{
	if (HealthComponent->GetIsDead() || !CurrentMeleeWeapon->IsValidLowLevelFast())
	{
		return;
	}
	AttackInput = AttackInput::LightAttack;
	if (bIsAttacking)
	{
		bSaveAttack = true;
	}
	else
	{
		bIsAttacking = true;
		DoAttack();
	}
}

void UPlayerAttackComponent::OnHeavyAttack()
{
	if (StaminaComponent->IsValidLowLevelFast())
	{
		if (StaminaComponent->GetCurrentAmountOfStamina() <= 1)
		{
			return;
		}
	}


	if (HealthComponent->GetIsDead() || !CurrentMeleeWeapon->IsValidLowLevelFast())
	{
		return;
	}
	AttackInput = AttackInput::HeavyAttack;
	if (bIsAttacking)
	{
		bSaveAttack = true;
	}
	else
	{
		bIsAttacking = true;
		DoAttack();
	}

}


void UPlayerAttackComponent::ComboAttack()
{
	if (!bSaveAttack)
	{
		return;
	}
	bSaveAttack = false;
	DoAttack();
}

void UPlayerAttackComponent::ResetCombo()
{
	AttackInput = AttackInput::None;
	AttackNumber = 0;
	bSaveAttack = false;
	bIsAttacking = false;
	Player->GetCharacterMovement()->MaxWalkSpeed = NormalMovementSpeed + NormalizedMultiplier * MaxAmountOfAddedSpeed;
}

void UPlayerAttackComponent::DoAttack()
{
	if (!CurrentMeleeWeapon->IsValidLowLevelFast())
	{
		return;
	}
	Player->GetCharacterMovement()->MaxWalkSpeed = NormalMovementSpeed * NormalizedSlowDownMovementspeedAttacking;
	float temp = 0;
	if (AttackInput == AttackInput::LightAttack)
	{
		// not copying the value
		const TArray<FAttacks>& LightAttacks = CurrentMeleeWeapon->GetLightAttacks();
		int32 arraySize = LightAttacks.Num();
		if (AttackNumber >= arraySize)
		{
			if (arraySize == 0)
			{
				bIsAttacking = false;
				return;
			}
			AttackNumber = 0;
		}
		FAttacks currentAttack = LightAttacks[AttackNumber];

		// doing an extra check if the montage is actually valid
		if (!currentAttack.Montage->IsValidLowLevelFast())
		{
			UE_LOG(LogTemp, Error, TEXT("AUE4PrototypeCharacter::DoAttack Montage on LightAttack %1 was invalid"), AttackNumber);
			bIsAttacking = false;
			return;
		}
		temp = Player->PlayAnimMontage(currentAttack.Montage, currentAttack.BaseAttackSpeed + NormalizedMultiplier * MaxAmountOfAddedAttackingSpeed);
		if (int(temp) == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("AUE4PrototypeCharacter::DoAttack PlayAnimMontage on LightAttack %1 failed"), AttackNumber);
		}
		CurrentMeleeWeapon->SetCurrentAttack(currentAttack);
	}
	else if (AttackInput == AttackInput::HeavyAttack)
	{
		const TArray<FAttacks>& HeavyAttacks = CurrentMeleeWeapon->GetHeavyAttacks();
		if (AttackNumber >= HeavyAttacks.Num())
		{
			if (HeavyAttacks.Num() == 0)
			{
				bIsAttacking = false;
				return;
			}

			AttackNumber = 0;
		}
		// stamina check 
		if (StaminaComponent->IsValidLowLevelFast())
		{
			if (StaminaComponent->GetCurrentAmountOfStamina() <= 1)
			{
				bIsAttacking = false;
				return;
			}
			else
			{
				StaminaComponent->UseStamina(2);
			}
		}
		FAttacks currentAttack = HeavyAttacks[AttackNumber];
		if (!currentAttack.Montage->IsValidLowLevelFast())
		{
			UE_LOG(LogTemp, Error, TEXT("AUE4PrototypeCharacter::DoAttack Montage on HeavyAttack $1 was invalid"), AttackNumber);
			bIsAttacking = false;
			return;
		}
		temp = Player->PlayAnimMontage(currentAttack.Montage, currentAttack.BaseAttackSpeed + NormalizedMultiplier * MaxAmountOfAddedAttackingSpeed);
		if (int(temp) == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("AUE4PrototypeCharacter::DoAttack PlayAnimMontage on HeavyAttack $1 failed"), AttackNumber);
		}
		CurrentMeleeWeapon->SetCurrentAttack(currentAttack);
	}
	AttackNumber++;
}

void UPlayerAttackComponent::SetCurrentWeapon(AMeleeWeapon* weapon)
{
	if (weapon->IsValidLowLevelFast())
	{
		CurrentMeleeWeapon = weapon;
	}
}
