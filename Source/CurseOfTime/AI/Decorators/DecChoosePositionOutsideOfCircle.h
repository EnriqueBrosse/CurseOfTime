// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "DecChoosePositionOutsideOfCircle.generated.h"

/**
 * 
 */
UCLASS()
class CURSEOFTIME_API UDecChoosePositionOutsideOfCircle : public UBTDecorator
{
	GENERATED_BODY()
		bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

};
