// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyAIController.h"
#include "SpawnerAIController.generated.h"

/**
 * 
 */
UCLASS()
class CURSEOFTIME_API ASpawnerAIController : public ABaseEnemyAIController
{
	GENERATED_BODY()
		FName SelfKey = "SelfActor"; 
public: 
	FName RandomPointInRoomKey = "RandomPointInRoom";
};
