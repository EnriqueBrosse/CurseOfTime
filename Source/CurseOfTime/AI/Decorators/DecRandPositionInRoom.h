// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "DecRandPositionInRoom.generated.h"

/**
 * 
 */

class ARoomController;
class LevelManager;
UCLASS()
class CURSEOFTIME_API UDecRandPositionInRoom : public UBTDecorator
{
	GENERATED_BODY()
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
private: 
	LevelManager* LevelManager;
};
