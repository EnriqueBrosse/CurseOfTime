// Fill out your copyright notice in the Description page of Project Settings.


#include "Pedestal.h"
#include "MeleeWeapon.h"
#include "../Characters/PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APedestal::APedestal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &APedestal::OnCollisionBeginOverlap);
	WeaponSpawn = CreateDefaultSubobject<UArrowComponent>("WeaponSpawn");
	WeaponSpawn->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APedestal::BeginPlay()
{
	Super::BeginPlay();
	const FTransform& transform = WeaponSpawn->GetComponentTransform();
	transform.GetLocation();
	MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(WeaponToSpawn, transform);
	FAttachmentTransformRules attachRules(EAttachmentRule::KeepWorld, true);

	MeleeWeapon->AttachToComponent(StaticMesh, attachRules);
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("APedestal::BeginPlay Player does not exist"));
	}
}


void APedestal::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Weapon") || OtherActor == MeleeWeapon)
	{
		return;
	}
	AMeleeWeapon* playerWeapon = Cast<AMeleeWeapon>(OtherActor);
	if (OtherActor->GetOwner() != Player)
	{
		return;
	}
	Player->SetWeapon(MeleeWeapon);
	MeleeWeapon = playerWeapon;
	FAttachmentTransformRules attachRules(EAttachmentRule::KeepWorld, true);
	MeleeWeapon->SetActorTransform(WeaponSpawn->GetComponentTransform());
	MeleeWeapon->AttachToComponent(StaticMesh, attachRules);
}