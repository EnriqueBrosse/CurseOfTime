// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/StatusEffectInterface.h"
#include "PlayerCharacter.generated.h"
// Made by Enrique Brosse

class UHealthComponent;
class UTimeWarpComponent;
class UPlayerAttackComponent;
class AMeleeWeapon;
class UDashComponent;
class UStaminaComponent;
class UKungFuGrid;

UCLASS()
class CURSEOFTIME_API APlayerCharacter : public ACharacter ,public IStatusEffectInterface
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UTimeWarpComponent* TimeWarpComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UDashComponent* DashComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaminaComponent* StaminaComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UPlayerAttackComponent* PlayerAttackComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UKungFuGrid* KungFuGrid; 

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMeleeWeapon>  Weapon1TypeToSpawnOnBeginPlay;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMeleeWeapon>  Weapon2TypeToSpawnOnBeginPlay;
		
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FName SocketWeapon1 = "";
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FName SocketWeapon2 = "";
	AMeleeWeapon* Weapon2 = nullptr;
	AMeleeWeapon* Weapon1 = nullptr;



	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundAttenuation* AttenuationSettings;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundConcurrency* ConcurrencySettings;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PlayerFeedback")
		UForceFeedbackEffect* ForceFeedback;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PlayerFeedback")
		UAnimMontage* StunMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PlayerFeedback")
		UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PlayerFeedback")
		UAnimMontage* hitMontage;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PlayerFeedback")
		UAnimMontage* SwitchMontage;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PlayerFeedback")
		float SwitchMontageSpeed = 1.5f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PlayerFeedback")
		float CutoffDeathMontage = 0.2f;
	UPROPERTY()
		FTimerHandle DeathTimeHandle;
public:
	// Sets default values for this character's properties
	APlayerCharacter();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		UFUNCTION()
	void SwitchWeapon();


	UFUNCTION(BlueprintCallable)
	void SwitchWeaponAnimCall();

	// interface functions 
	virtual void OnDeath() override;
	virtual void OnStun() override;
	virtual void OnKnockBack(const FVector& knockbackValue)override;
	virtual void OnShieldHit() override;

	virtual void OnHit()override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PauseGame();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UnPauseGame();


	void SetWeapon(AMeleeWeapon* weapon);

	UFUNCTION(BlueprintCallable)
		AMeleeWeapon* GetCurrentWeapon()const;
	UFUNCTION(BlueprintCallable)
		AMeleeWeapon* GetOtherWeapon()const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		bool bIsPaused;
private: 
	void MoveForward(float Value);
	void MoveRight(float Value);
	UFUNCTION()
		void CheckDeath();


};
