// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTMoveToImproved.generated.h"

class UHealthComponent;
/**
 * 
 */
UCLASS()
class CURSEOFTIME_API UBTMoveToImproved : public UBTTask_MoveTo
{
	GENERATED_BODY()
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

	UHealthComponent* HealthComp;
};
