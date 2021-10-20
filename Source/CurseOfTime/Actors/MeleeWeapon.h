// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeWeapon.generated.h"

USTRUCT(BlueprintType)
struct FAttacks
{
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
		float BaseAttackSpeed;
	UPROPERTY(EditAnywhere)
		int Damage = 0;
	UPROPERTY(EditAnywhere)
		int LifeSteal = 0;
	UPROPERTY(EditAnywhere)
		bool bStun = false;
	UPROPERTY(EditAnywhere)
		bool bKnockback = false;
	UPROPERTY(EditAnywhere)
		float KnockbackValue = 0;

	// if we add more status effects add them here 

	// this is a tunable variable for how strong an attack is
	UPROPERTY(EditAnywhere, Category = KungFuCircle)
		int AttackWeight; 
};

class UStaticMeshComponent;
class UHealthComponent;
class UCapsuleComponent;
class UBoxComponent;
class ACurseOfTimeGameModeBase;
class UNiagaraComponent;

UCLASS()
class CURSEOFTIME_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeleeWeapon();

	// turning on the collision in the box, will be set in an event
	UFUNCTION(BlueprintCallable)
		void TurnOnCollsion();

	// turning off the collision in the box, will be set in an event
	UFUNCTION(BlueprintCallable)
		void TurnOffCollision();
	
	void SetCurrentAttack(const FAttacks& attack);

	FORCEINLINE const FAttacks& GetCurrentAttack()const { return CurrentAttack; }

	// will add the owner variable and the healthComponent
	void AddOwner(); 

	// will set the owner and healthcomponent to nullptr 
	void RemoveOwner();

	FORCEINLINE const TArray<FAttacks>& GetLightAttacks()const { return LightAttacks; }
	FORCEINLINE const TArray<FAttacks>& GetHeavyAttacks()const { return HeavyAttacks; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE UTexture2D* GetUITexture()const { return UITexture; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE UTexture2D* GetUITextureUnEquiped()const { return UITextureUnEquiped; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TArray<FAttacks> LightAttacks;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TArray<FAttacks> HeavyAttacks;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		bool bHasOwner = false;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
		UTexture2D* UITexture; 
	UPROPERTY(EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
		UTexture2D* UITextureUnEquiped;

	UPROPERTY(EditAnywhere, Category = "PlayerFeedback", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* NiagaraComponent; 

	UHealthComponent* PlayerHealthComponent;
	FAttacks CurrentAttack;
	AActor* Owner;

	FName Tag;

	ACurseOfTimeGameModeBase* GameMode;


	TArray<AActor*> ActorsHit;

	UFUNCTION()
		void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void CheckIfAttacksAreValid();
	void ShieldHit(AActor* otherActor); 
};
