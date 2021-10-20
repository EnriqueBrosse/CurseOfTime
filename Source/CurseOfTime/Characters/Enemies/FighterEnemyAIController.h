// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyAIController.h"
#include "FighterEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class CURSEOFTIME_API AFighterEnemyAIController : public ABaseEnemyAIController
{
	GENERATED_BODY()
public:
		// blackboard keys 
	FName SelfActorKey = "SelfActor";
	FName NodePositionKey = "NodePosition";
	FName NodeIndexKey = "NodeIndex";
	FName AttackKey = "Attack";
	FName PointOutsideCircleKey = "PointOutsideCircle";

};
