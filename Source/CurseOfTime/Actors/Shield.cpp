// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"

// Sets default values
AShield::AShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(BoxComponent);
	Tags.Add("Shield");
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldHit"));
	NiagaraComp->SetupAttachment(RootComponent);
}

void AShield::BeginPlay()
{
	Super::BeginPlay();
	NiagaraComp->DeactivateImmediate();
}

void AShield::TurnOnCollision()
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

void AShield::TurnOffCollision()
{
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShield::TurnOnNiagara()
{
	if (NiagaraComp)
	{
		NiagaraComp->Activate();
	}
}