// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockerEnemy.h"
#include "../../Actors/Shield.h"
#include "Kismet/GameplayStatics.h"

ABlockerEnemy::ABlockerEnemy()
	:AFighterEnemy(){}


void ABlockerEnemy::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParameters.Owner = this;
	FAttachmentTransformRules attachmentRulesWeapon{ EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,true };
	if (ShieldToSpawn != NULL)
	{
		FTransform tranformWeapon = GetMesh()->GetSocketTransform(SocketShield, ERelativeTransformSpace::RTS_ParentBoneSpace);
		Shield = GetWorld()->SpawnActor<AShield>(ShieldToSpawn, tranformWeapon, spawnParameters);
		Shield->AttachToComponent(GetMesh(), attachmentRulesWeapon, SocketShield);
	}
	Tags.Add("ShieldEnemy");
}

void ABlockerEnemy::SetIsActive(bool isActive)
{
	Super::SetIsActive(isActive);
	if (!Shield)
	{
		return;
	}
	Shield->SetActorHiddenInGame(!isActive);

	if (isActive)
	{
		Shield->TurnOnCollision();
	}
	else
	{
		Shield->TurnOffCollision();
	}
}


void ABlockerEnemy::OnShieldHit()
{
	// make it play a sound 
	if (ShieldHitSound && AttenuationSettings && ConcurrencySettings)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShieldHitSound, Shield->GetActorLocation(), Shield->GetActorRotation(),
			1.f, 1.f, 0.f, AttenuationSettings, ConcurrencySettings, GetOwner());
	}

	//make it play an animation 
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
	bool foundPlayingAnim = false;
	TArray<FAttacks> attacks = MeleeWeapon->GetLightAttacks();
	const TArray<FAttacks>& heavyAttacks = MeleeWeapon->GetHeavyAttacks();
	for (const FAttacks& heavyAttack : heavyAttacks)
	{
		attacks.Add(heavyAttack);
	}
	// check if an attacking animation is playing 
	for (const FAttacks& attack : attacks)
	{
		if (animInstance->Montage_IsPlaying(attack.Montage))
		{
			foundPlayingAnim = true; 
			break;
		}
	}
	
	if (!foundPlayingAnim)
	{
		MeleeWeapon->TurnOffCollision();
		PlayAnimMontage(ShieldHit);
	}

	Shield->TurnOnNiagara();
}