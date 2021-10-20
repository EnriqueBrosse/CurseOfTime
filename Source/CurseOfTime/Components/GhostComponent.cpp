// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TimeWarpComponent.h"

// Sets default values for this component's properties
UGhostComponent::UGhostComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGhostComponent::BeginPlay()
{
	Super::BeginPlay();
	TimeWarpComponent = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UTimeWarpComponent>();
	if (!TimeWarpComponent->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("UGhostComponent::BeginPlay player does not have a timewarpcomponent"))
			return;
	}
	Owner = GetOwner();
	// this data is not going to change after beginplay 
	AmountOfCapturesPerSecond = TimeWarpComponent->GetAmountOfCaptures();
}


// Called every frame
void UGhostComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!TimeWarpComponent->IsValidLowLevel())
	{
		return;
	}
	Owner->SetActorHiddenInGame(!TimeWarpComponent->GetCanReverse());
	float interpolation = TimeWarpComponent->GetTimeUntilNextFrameCapture() / (1.f / AmountOfCapturesPerSecond);
	const std::pair<UTimeWarpComponent::DataToBeSaved, UTimeWarpComponent::DataToBeSaved>& data = TimeWarpComponent->GetFirstDequeResult();

	FTransform newTransform;
	newTransform.SetLocation(FMath::Lerp(data.second.Transform.GetLocation(), data.first.Transform.GetLocation(), interpolation));
	newTransform.SetRotation(FMath::Lerp(data.second.Transform.GetRotation(), data.first.Transform.GetRotation(), interpolation));
	newTransform.SetScale3D(FMath::Lerp(data.second.Transform.GetScale3D(), data.first.Transform.GetScale3D(), interpolation));
	Owner->SetActorTransform(newTransform,false,nullptr,ETeleportType::TeleportPhysics);

	// ...
}

