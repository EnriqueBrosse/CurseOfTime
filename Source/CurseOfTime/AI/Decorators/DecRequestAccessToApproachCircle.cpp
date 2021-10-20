// Fill out your copyright notice in the Description page of Project Settings.


#include "DecRequestAccessToApproachCircle.h"
#include "GameFramework/Character.h"
#include "../../Characters/Enemies/FighterEnemy.h"
#include "../../Characters/Enemies/FighterEnemyAIController.h"
#include "../../Actors/EnemyManager.h"

bool UDecRequestAccessToApproachCircle::CalculateRawConditionValue
	(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter* character = OwnerComp.GetAIOwner()->GetCharacter();
	AFighterEnemyAIController* controller = Cast<AFighterEnemyAIController>(OwnerComp.GetAIOwner());
	if (character->IsValidLowLevelFast() && controller->IsValidLowLevelFast())
	{
		AFighterEnemy* enemy = Cast<AFighterEnemy>(character);
		if (enemy->IsValidLowLevelFast() && enemy->GetEnemyManager()->IsValidLowLevelFast())
		{
			int nodeIndex = 0;
			return enemy->GetEnemyManager()->RequestAccesToApproachCircle(enemy);
		}
	}
	return false;
}
