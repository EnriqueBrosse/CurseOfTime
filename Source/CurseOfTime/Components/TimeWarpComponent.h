// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <deque>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeWarpComponent.generated.h"

class UHealthComponent;
class USoundAttenuation;
class USoundConcurrency;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTIME_API UTimeWarpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeWarpComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	struct DataToBeSaved
	{
		FTransform Transform;
		float Health;
	};

	UFUNCTION()
	void Reverse();
	UFUNCTION()
	void Reset();

	UFUNCTION(BlueprintCallable)
	float GetMaxAmountOfSecondsDelay()const { return AmountOfSecondsDelay; }
	UFUNCTION(BlueprintCallable)
		float GetCurrentDelay()const { return CurrentDelay; }

	std::pair<DataToBeSaved, DataToBeSaved> GetFirstDequeResult() const;
	float GetTimeUntilNextFrameCapture()const { return TimeUntilNextFrameCapture; }
	int GetAmountOfCaptures()const { return AmountOfCapturesPerSecond; }

	bool GetCanReverse()const { return bCanReverse; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	void SaveData(float DeltaTime);
	void AddData();


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int AmountOfSecondsDelay = 1; 

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int AmountOfCapturesPerSecond = 10;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundBase* TimeWarpSound;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundAttenuation* AttenuationSettings;
		UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Sound")
		USoundConcurrency* ConcurrencySettings;

	std::deque<DataToBeSaved> DataDeque;
	UHealthComponent* HealthComponent;
	int Maxcount;
	int DequeCount;
	AActor* Owner;
	float TimeUntilNextFrameCapture;
	float CurrentDelay = 0;
	bool bCanReverse = false;		
};
