// Fill out your copyright notice in the Description page of Project Settings.


#include "BTMoveToImproved.h"
#include "../../Components/HealthComponent.h"

EBTNodeResult::Type UBTMoveToImproved::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!HealthComp)
	{
		HealthComp = OwnerComp.GetOwner()->FindComponentByClass<UHealthComponent>();
	}
	if (HealthComp && HealthComp->GetIsDead())
	{
		return EBTNodeResult::Failed;
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}


void UBTMoveToImproved::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (HealthComp && HealthComp->GetIsDead())
	{
		Super::AbortTask(OwnerComp, NodeMemory);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
