// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTPlayMontage.h"
#include "BTSpawnEnemy.generated.h"

class UHealthComponent;
UCLASS()
class CURSEOFTIME_API UBTSpawnEnemy : public UBTPlayMontage
{
	GENERATED_BODY()
		EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
private:
	UHealthComponent* HealthComponent;

};
