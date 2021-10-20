// Copyright Epic Games, Inc. All Rights Reserved.
// Made by Enrique Brosse

#include "CurseOfTimeGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayerCharacter.h"
#include "Components/PlayerAttackComponent.h"
#include "UObject/ConstructorHelpers.h"

ACurseOfTimeGameModeBase::ACurseOfTimeGameModeBase()
{
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/BP_PlayerCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
	//making sure this can tick 
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}


void ACurseOfTimeGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AmountOfTimeBeforeResetCombo -= DeltaTime;
	if (AmountOfTimeBeforeResetCombo < DeltaTime)
	{
		ResetMultipyer();
	}
}

void ACurseOfTimeGameModeBase::AddScore(int score)
{
	Score += score * ScoreMultiplier;
}

void ACurseOfTimeGameModeBase::ResetMultipyer()
{
	if (!Player)
	{
		Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (!PlayerAttackComponent)
		{
			PlayerAttackComponent = Player->FindComponentByClass<UPlayerAttackComponent>();
		}
	}
	ScoreMultiplier = 1;
	AmountOfHits = 0;
	PlayerAttackComponent->SetNormalizedMultiplier(0);
}

void ACurseOfTimeGameModeBase::AddHit()
{
	AmountOfHits++;
	AmountOfTimeBeforeResetCombo = MaxAmountOfTimeBeforeResetCombo;
	// looking on what multiplyer we're on
	for (size_t i = 0; i < ScoreMultiplierLookupTable.Num(); i++)
	{
		if (ScoreMultiplierLookupTable[i].Multiplier > AmountOfHits)
		{
			if (i > 0)
			{
				ScoreMultiplier = ScoreMultiplierLookupTable[i].AmountOfHits;
				const float normalizedScore = float(ScoreMultiplierLookupTable[i].Multiplier) / float(ScoreMultiplierLookupTable[ScoreMultiplierLookupTable.Num() - 1].Multiplier);
				if (!Player)
				{
					Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				}
				PlayerAttackComponent->SetNormalizedMultiplier(normalizedScore);
			}
			else
			{
				ScoreMultiplier = 1;
			}
			return;
		}
	}

}

void ACurseOfTimeGameModeBase::StartPlay()
{
	Super::StartPlay();
	Score = 0;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACurseOfTimeGameModeBase::RemoveScore, 1, true);
}

void ACurseOfTimeGameModeBase::RemoveScore()
{
	if (Score != 0)
	{
		Score--;
	}
}

void ACurseOfTimeGameModeBase::RemoveScoreGoingDown()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}