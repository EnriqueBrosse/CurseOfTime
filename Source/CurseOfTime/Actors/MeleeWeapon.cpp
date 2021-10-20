// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "../Components/HealthComponent.h"
#include "../Characters/PlayerCharacter.h"
#include "../Interfaces/StatusEffectInterface.h"
#include "../CurseOfTimeGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetupAttachment(RootComponent);
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnCollisionBeginOverlap);
	Tags.Add("Weapon");
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (bHasOwner)
	{
		AddOwner();
	}
	else
	{
		RemoveOwner();
	}

	GameMode = Cast<ACurseOfTimeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	NiagaraComponent->DeactivateImmediate();
}

void AMeleeWeapon::CheckIfAttacksAreValid()
{
	for (size_t i = 0; i < LightAttacks.Num(); i++)
	{
		if (!LightAttacks[i].Montage->IsValidLowLevelFast())
		{
			UE_LOG(LogTemp, Error, TEXT("AnimMontageLightAttack %i does not exist in this object: %s"), i, *GetName());

		}
	}

	for (size_t i = 0; i < HeavyAttacks.Num(); i++)
	{
		if (!HeavyAttacks[i].Montage->IsValidLowLevelFast())
		{
			UE_LOG(LogTemp, Error, TEXT("AnimMontageHeavyAttack %i does not exist in this object: %s"), i, *GetName());
		}
	}
}

void AMeleeWeapon::SetCurrentAttack(const FAttacks& attack)
{
	CurrentAttack = attack;
}

void AMeleeWeapon::AddOwner()
{
	Owner = GetOwner();
	if (!Owner->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("AMeleeWeapon::AddOwner() does not exist %s"), *GetName())
	}
	else
	{
		bHasOwner = true;
	}

	PlayerHealthComponent = Owner->FindComponentByClass<UHealthComponent>();
	if (!PlayerHealthComponent->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("AMeleeWeapon::AddOwner() owner does not have a healthcomponent %s"), *GetName())
	}
	TurnOffCollision();

	if (Owner->ActorHasTag("Player"))
	{
		Tag = "Player";
	}
	else if (Owner->ActorHasTag("Enemy"))
	{
		Tag = "Enemy";
	}
	else
	{
		Tag = "";
	}

	
	//FTimerHandle timeHandle1;
	//FTimerHandle timeHandle2;
	//GetWorldTimerManager().SetTimer(timeHandle1, this, &AMeleeWeapon::TurnOffCollision, 0.5f, false);
}

void AMeleeWeapon::RemoveOwner()
{
	TurnOffCollision();
	PlayerHealthComponent = nullptr;
	Owner = nullptr;
	bHasOwner = false;
	TurnOffCollision();
	Tag = "";

	//FTimerHandle timeHandle1;
	//FTimerHandle timeHandle2;
	//GetWorldTimerManager().SetTimer(timeHandle1, this, &AMeleeWeapon::TurnOnCollsion, 0.5f,false);
}

void AMeleeWeapon::TurnOnCollsion()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//UE_LOG(LogTemp, Error, TEXT("AMeleeWeapon::TurnOnCollsion Collision enabled %s"), *GetName());
}

void AMeleeWeapon::TurnOffCollision()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//UE_LOG(LogTemp, Error, TEXT("AMeleeWeapon::TurnOffCollision Collision enabled %s"), *GetName());
	ActorsHit.Empty();
}

void AMeleeWeapon::ShieldHit(AActor* otherActor)
{
	if (ActorsHit.Find(otherActor) == INDEX_NONE)
	{
		ActorsHit.Add(otherActor);
		UHealthComponent* healthComp = otherActor->FindComponentByClass<UHealthComponent>();
		if (healthComp && healthComp->GetIsDead())
		{
			return;
		}

		IStatusEffectInterface* statusEffectInterfacle = Cast<IStatusEffectInterface>(otherActor);
		if (statusEffectInterfacle)
		{
			statusEffectInterfacle->OnShieldHit();
		}
	}
}

void AMeleeWeapon::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bHasOwner)
	{
		return;
	}

	if (Owner == OtherActor)
	{
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("%s hit %s"), *GetName(), *OtherActor->GetName());

	// checking if an enemy hit another enemy or a wall or something
	if (OtherActor->ActorHasTag(Tag) || OtherActor->Tags.Num() == 0)
	{
		return;
	}

	UHealthComponent* healthComponent = OtherActor->FindComponentByClass<UHealthComponent>();
	if (!healthComponent->IsValidLowLevelFast()) // faster nullptr check
	{
		if (OtherActor->ActorHasTag("Shield") && OtherActor->GetOwner() != Owner)
		{
			ShieldHit(OtherActor->GetOwner());
		}
		return;
	}

	if (ActorsHit.Find(OtherActor) != INDEX_NONE)
	{
		return;
	}


	//if (OtherActor->ActorHasTag("ShieldEnemy"))
	//{
	//	const FVector& shieldEnemyDirection = OtherActor->GetActorForwardVector();
	//	const FVector& playerDirection = GetOwner()->GetActorForwardVector();
	//	const float angle = FMath::Acos(FVector::DotProduct(shieldEnemyDirection, playerDirection));
	//	const float angleToHitShield = FMath::DegreesToRadians<float>(180.f);
	//	if (angle > angleToHitShield)
	//	{
	//		ShieldHit(OtherActor);
	//		return;
	//	}
	//}


	ActorsHit.Add(OtherActor->GetOwner());


	GameMode->AddHit();

	healthComponent->DoDamage(CurrentAttack.Damage, true);
	// checking here if it's not death because we don't want to play an animation 
	if (healthComponent->GetIsDead())
	{
		return;
	}
	NiagaraComponent->Activate(true);
	// add the lifesteal
	PlayerHealthComponent->Heal(CurrentAttack.LifeSteal);
	// check if it has the interface so I can add the effects if necesarry 
	IStatusEffectInterface* statusEffectInterfacle = Cast<IStatusEffectInterface>(OtherActor);
	if (statusEffectInterfacle == nullptr)
	{
		return;
	}
	// if we add more status effects add them here 
	if (CurrentAttack.bStun)
	{
		statusEffectInterfacle->OnStun();
	}
	if (CurrentAttack.bKnockback)
	{
		statusEffectInterfacle->OnKnockBack(Owner->GetActorForwardVector() * CurrentAttack.KnockbackValue);
	}
}