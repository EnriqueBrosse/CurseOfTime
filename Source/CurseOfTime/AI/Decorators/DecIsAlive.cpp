// Fill out your copyright notice in the Description page of Project Settings.


#include "DecIsAlive.h"
#include "GameFramework/Character.h"
#include "../../Components/HealthComponent.h"
#include "AIController.h"

bool UDecIsAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter* character = OwnerComp.GetAIOwner()->GetCharacter();
	if (character->IsValidLowLevelFast())
	{
		UHealthComponent* healthComp = character->FindComponentByClass<UHealthComponent>();
		if (healthComp->IsValidLowLevelFast())
		{
			return !healthComp->GetIsDead();
		}
	}
	return false;
}
