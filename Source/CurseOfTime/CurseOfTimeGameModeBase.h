// Copyright Epic Games, Inc. All Rights Reserved.
// Made by Enrique Brosse

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CurseOfTimeGameModeBase.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMultiplierScore
{
	GENERATED_USTRUCT_BODY()
public: 
	UPROPERTY(EditAnywhere)
		int Multiplier; 
	UPROPERTY(EditAnywhere)
		int AmountOfHits;
};

class APlayerCharacter;
class UPlayerAttackComponent;

UCLASS()
class CURSEOFTIME_API ACurseOfTimeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public: 
	ACurseOfTimeGameModeBase();
	virtual void StartPlay() override;

	virtual void Tick(float DeltaTime) override;

	// adding score
	void AddScore(int score);

	// Resetting the muliplyer and the amount of hits 
	void ResetMultipyer();

	// adding a hit and looking if it pushes us in another combo multiplyer
	void AddHit();

	void RemoveScoreGoingDown();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
		int Score = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
		int ScoreMultiplier = 1;
	// first int is the muliplyer the other one is the amount of hits it takes to get to the next one
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TArray<FMultiplierScore> ScoreMultiplierLookupTable;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float MaxAmountOfTimeBeforeResetCombo;

	int AmountOfHits = 0;
	float AmountOfTimeBeforeResetCombo;
	APlayerCharacter* Player;
	UPlayerAttackComponent* PlayerAttackComponent;

	UFUNCTION()
		void RemoveScore();
	FTimerHandle TimerHandle;
};
