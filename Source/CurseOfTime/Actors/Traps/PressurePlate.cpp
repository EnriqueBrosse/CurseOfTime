// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"
#include "BaseTrap.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	this->SetRootComponent(FloorMesh);
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnOverlapBeginBox);

}

void APressurePlate::OnOverlapBeginBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (size_t i = 0; i < TrapArray.Num(); i++)
	{
		if (TrapArray[i] == OtherActor)
		{
			return;
		}
	}
	// checking if it overlaps with a player/enemy
	if (!Cast<ACharacter>(OtherActor))
	{
		return;
	}

	for (size_t i = 0; i < TrapArray.Num(); i++)
	{
		TrapArray[i]->Execute();
	}
}
