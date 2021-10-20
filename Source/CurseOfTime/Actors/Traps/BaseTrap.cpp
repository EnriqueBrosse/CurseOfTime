// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTrap.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseTrap::ABaseTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
}

void ABaseTrap::BeginPlay()
{
	Super::BeginPlay();
	NiagaraComponent->DeactivateImmediate();
}

void ABaseTrap::Execute()
{
	if (NiagaraComponent)
	{
		if (NiagaraSpawnDelay <= 0.001)
		{
			ExecuteNiagara();
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(NiagaraTimeHandle, this, &ABaseTrap::ExecuteNiagara, NiagaraSpawnDelay, false);
		}
	}

	if (TrapSound && AttenuationSettings && ConcurrencySettings)
	{
		if (SoundDelay <= 0.001)
		{
			ExecuteSound();
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(SoundTimeHandle, this, &ABaseTrap::ExecuteSound, SoundDelay, false);
		}
	}


}

void ABaseTrap::ExecuteNiagara()
{
	NiagaraComponent->Activate(true);
}

void ABaseTrap::ExecuteSound()
{
	UGameplayStatics::PlaySoundAtLocation(this, TrapSound, GetActorLocation(), GetActorRotation(),
		1.f, 1.f, 0.f, AttenuationSettings, ConcurrencySettings, this);
}
