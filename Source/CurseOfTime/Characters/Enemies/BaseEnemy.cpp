// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "BaseEnemyAIController.h"
#include "../../Components/HealthComponent.h"
#include "../../Actors/MeleeWeapon.h"
#include "../../CurseOfTimeGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	Tags.Add("Enemy");
	SpawnNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SpawnNiagaraComp"));
	DespawnNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DespawnNiagaraComp"));
	SpawnNiagaraComp->SetupAttachment(RootComponent);
	DespawnNiagaraComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	SetIsActive(true);
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ABaseEnemy::OnSeePawn);
	SpawnNiagaraComp->DeactivateImmediate();
	DespawnNiagaraComp->DeactivateImmediate();

}

void ABaseEnemy::OnDeath()
{
	ACurseOfTimeGameModeBase* gameMode = Cast<ACurseOfTimeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gameMode->IsValidLowLevelFast())
	{
		gameMode->AddScore(ScoreOnKill);
	}

	if (DeathSound && AttenuationSettings && ConcurrencySettings)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation(),
			1.f, 1.f, 0.f, AttenuationSettings, ConcurrencySettings, this);
	}

	if (!DeathMontage->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("ABaseEnemy::OnDeath stunmontage is invalid"));
		return;
	}
	StopAnimMontage();
	float check = PlayAnimMontage(DeathMontage);
	if (check == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ABaseEnemy::OnStun PlayAnimMontage is invalid"));
	}
	else
	{
		FTimerHandle dathTimeHandle; 
		// delay until death animation has played  
		GetWorldTimerManager().SetTimer(dathTimeHandle, this, &ABaseEnemy::Reset, check - CutoffDeathMontage, false);
	}
	if (DespawnNiagaraComp)
	{
		FTimerHandle despawnTimerHandle;
		// delay until death animation has played  
		GetWorldTimerManager().SetTimer(despawnTimerHandle, this, &ABaseEnemy::SpawnDeaspawnNiagara, check - DespawnNiagaraDelayFromAnim, false);

	}

}

void ABaseEnemy::OnKnockBack(const FVector& knockbackValue)
{
	LaunchCharacter(knockbackValue, true, false);
}

void ABaseEnemy::OnStun()
{
	if (!StunMontage->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("ABaseEnemy::OnStun stunmontage is invalid"));
		return;
	}
	StopAnimMontage();
	float check = PlayAnimMontage(StunMontage);
	
	if (check == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ABaseEnemy::OnStun PlayAnimMontage is invalid"));
	}
}

void ABaseEnemy::OnHit()
{
	if (HitSound && AttenuationSettings && ConcurrencySettings)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(), GetActorRotation(),
			1.f, 1.f, 0.f, AttenuationSettings, ConcurrencySettings, this);
	}

	if (hitMontage)
	{
		StopAnimMontage();
		PlayAnimMontage(hitMontage);
	}
}

void ABaseEnemy::OnShieldHit()
{
}

void ABaseEnemy::Reset()
{
	// set on non active 
	SetIsActive(false);
}

void ABaseEnemy::SetIsActive(bool isActive)
{
	bIsActive = isActive;
	//GetCharacterMovement()->bUseRVOAvoidance = isActive;
	SetActorHiddenInGame(!bIsActive);
	// setting the collsion presets to it doesn't fall out of the map
	PawnSensingComponent->bEnableSensingUpdates = bIsActive;
	if (isActive)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		HealthComponent->Reset();
		if (SpawnNiagaraComp)
		{
			SpawnNiagaraComp->Activate(true);
		}
	}
	else
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
}

void ABaseEnemy::OnSeePawn(APawn* seenPawn)
{
	if (seenPawn == Player)
	{
		ABaseEnemyAIController* baseController = Cast<ABaseEnemyAIController>(GetController());
		if (baseController->IsValidLowLevelFast())
		{
			// do something
			baseController->PlayerSensed();
		}
	}
}

bool ABaseEnemy::GetIsDead()const
{
	return HealthComponent->GetIsDead();
}

void ABaseEnemy::SpawnDeaspawnNiagara()
{
	DespawnNiagaraComp->Activate(true);
}
