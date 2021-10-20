// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeTrap.h"
#include "Components/StaticMeshComponent.h"
#include "../../Components/HealthComponent.h"
#include "NiagaraComponent.h"

// Sets default values
ASpikeTrap::ASpikeTrap() : ABaseTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpikeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpikeMesh"));
	SetRootComponent(SpikeMesh);
	NiagaraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();

	if (!SpikeMesh->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("ASpikeTrap::BeginPlay() SpikeMesh is invalid %s"), *GetName());
		return;
	}
	SpikeMesh->OnComponentBeginOverlap.AddDynamic(this, &ASpikeTrap::OnOverlapBeginSpikes);
}

// Called every frame
void ASpikeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeLine.TickTimeline(DeltaTime);
}

void ASpikeTrap::Execute()
{
	Super::Execute();
	UE_LOG(LogTemp, Warning, TEXT("Trap overlap"));

	//if (OtherActor == this)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("ASpikeTrap::OnOverlapBeginBox() overlapping with itself"));
	//	return;
	//}
	// if it's playing we don't need to reset it
	if (TimeLine.IsPlaying())
	{
		return;
	}

	if (!CurveFloat->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("ASpikeTrap::BeginPlay() CurveFloat is invalid"));
		return;
	}
	FOnTimelineFloat TimeLineProgress;
	TimeLineProgress.BindUFunction(this, FName("TimelineProgress"));
	TimeLine.AddInterpFloat(CurveFloat, TimeLineProgress);
	TimeLine.SetLooping(false);
	StartLocation = SpikeMesh->GetRelativeLocation();
	EndLocation = StartLocation;
	EndLocation += SpikeMesh->GetUpVector() * SpikeUpOffset;
	TimeLine.PlayFromStart();
}

void ASpikeTrap::OnOverlapBeginSpikes(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Damage"));
	if (OtherActor == this)
	{
		UE_LOG(LogTemp, Error, TEXT("ASpikeTrap::OnOverlapBeginSpikes() overlapping with itself change collision settings"));
		return;
	}
	UHealthComponent* healthComp = OtherActor->FindComponentByClass<UHealthComponent>();
	if (!healthComp->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("ASpikeTrap::OnOverlapBeginSpikes() %s didn't have an healthComponent"), *OtherActor->GetName());
		return;
	}
	healthComp->DoDamage(Damage, true);
}

void ASpikeTrap::TimelineProgress(float value)
{
	FVector location = FMath::Lerp(StartLocation, EndLocation, value);
	SpikeMesh->SetRelativeLocation(location);
}
