// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTPlayMontage.h"
#include "BTAttack.generated.h"

class UHealthComponent;
/**
 * 
 */
UCLASS()
class CURSEOFTIME_API UBTAttack : public UBTPlayMontage
{
	GENERATED_BODY()
		EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
		void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

private: 
	UHealthComponent* HealthComponent;
};
