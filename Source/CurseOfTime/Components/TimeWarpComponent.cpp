// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeWarpComponent.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UTimeWarpComponent::UTimeWarpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UTimeWarpComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	Maxcount = AmountOfCapturesPerSecond * AmountOfSecondsDelay;
	Owner = GetOwner();
	if (!Owner->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("UReverseTimeComponent does not have a owner"))
			return;
	}
	HealthComponent = Owner->FindComponentByClass<UHealthComponent>();
	if (!HealthComponent->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("UTimeWarpComponent::BeginPlay() owner does not have a healthComponent %s"),*Owner->GetName())
			return;
	}
	CurrentDelay = 0;
}


// Called every frame
void UTimeWarpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SaveData(DeltaTime);
	// ...
}

void UTimeWarpComponent::SaveData(float DeltaTime)
{
	CurrentDelay += DeltaTime;
	if (CurrentDelay >= AmountOfSecondsDelay)
	{
		CurrentDelay = AmountOfSecondsDelay;
		bCanReverse = true;
	}
	TimeUntilNextFrameCapture -= DeltaTime;
	if (TimeUntilNextFrameCapture <= 0)
	{
		TimeUntilNextFrameCapture = 1.f / AmountOfCapturesPerSecond;
		if (DequeCount >= Maxcount)
		{
			if (!DataDeque.empty())
			{
				DataDeque.pop_front();
				DequeCount--;
			}
		}
		AddData();
	}
}

void UTimeWarpComponent::AddData()
{
	DataToBeSaved toBeSaved;
	toBeSaved.Transform = Owner->GetActorTransform();
	toBeSaved.Health = HealthComponent->GetCurrentHealth();;
	DataDeque.push_back(toBeSaved);
	DequeCount++;
}

void UTimeWarpComponent::Reverse()
{
	if (DataDeque.empty() || !bCanReverse)
	{
		return;
	}
	// add player feedback
	UGameplayStatics::PlaySoundAtLocation(this, TimeWarpSound, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(),
		1.f, 1.f, 0.f, AttenuationSettings, ConcurrencySettings, GetOwner());

	// set player data 
	DataToBeSaved data = DataDeque.front();
	Owner->SetActorTransform(data.Transform);
	bool wasdeath = HealthComponent->GetIsDead();
	HealthComponent->SetHealth(data.Health);
	if (wasdeath && !HealthComponent->GetIsDead())
	{
		ACharacter* character = Cast<ACharacter>(Owner);
		character->StopAnimMontage();
	}
	// reset all the data
	Reset();
}

void UTimeWarpComponent::Reset()
{
	bCanReverse = false;
	DequeCount = 0;
	CurrentDelay = 0;
	DataDeque.clear();
	TimeUntilNextFrameCapture = 1.f / AmountOfCapturesPerSecond;
}

std::pair<UTimeWarpComponent::DataToBeSaved, UTimeWarpComponent::DataToBeSaved> UTimeWarpComponent::GetFirstDequeResult() const
{
	if (DequeCount >= 2)
	{
		FTransform nowTransform = Owner->GetActorTransform();
		if (!DataDeque.front().Transform.Equals(nowTransform))
		{
			return std::make_pair(DataDeque.front(), DataDeque[1]);
		}
	}
	// setting it outside of the map
	DataToBeSaved data;
	data.Transform.SetLocation(FVector(0, 0, -1000));
	return std::make_pair(data, data);
}


