// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterEnemy.h"
#include "Kismet/GameplayStatics.h"

void AFighterEnemy::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParameters.Owner = this;
	FAttachmentTransformRules attachmentRulesWeapon{ EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,true };
	if (WeaponToSpawnOnBeginPlay != NULL)
	{
		FTransform tranformWeapon = GetMesh()->GetSocketTransform(SocketWeapon, ERelativeTransformSpace::RTS_ParentBoneSpace);
		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(WeaponToSpawnOnBeginPlay, tranformWeapon, spawnParameters);
		MeleeWeapon->AttachToComponent(GetMesh(), attachmentRulesWeapon, SocketWeapon);
		MeleeWeapon->AddOwner();
	}
}

void AFighterEnemy::SetIsActive(bool isActive)
{
	Super::SetIsActive(isActive);
	if (MeleeWeapon)
	{
		MeleeWeapon->SetActorHiddenInGame(!isActive);
		if (!isActive)
		{
			MeleeWeapon->TurnOffCollision();
		}
	}
}

void AFighterEnemy::SetCurrentAttack(const FAttacks& currentAttack)
{
	MeleeWeapon->SetCurrentAttack(currentAttack);
}

void AFighterEnemy::OnHit()
{
	if (HitSound && AttenuationSettings && ConcurrencySettings)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(), GetActorRotation(),
			1.f, 1.f, 0.f, AttenuationSettings, ConcurrencySettings, this);
	}

	if (!hitMontage)
	{
		return;
	}
	const FAttacks& attack = MeleeWeapon->GetCurrentAttack(); 
	if (attack.Montage == nullptr)
	{
		return;
	}
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(attack.Montage))
	{
		return;
	}
	StopAnimMontage();
	PlayAnimMontage(hitMontage);
	MeleeWeapon->TurnOffCollision();

}

void AFighterEnemy::OnDeath()
{
	Super::OnDeath(); 
	MeleeWeapon->TurnOffCollision();
}