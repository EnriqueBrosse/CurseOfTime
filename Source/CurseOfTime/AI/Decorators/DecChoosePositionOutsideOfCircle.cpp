// Fill out your copyright notice in the Description page of Project Settings.


#include "DecChoosePositionOutsideOfCircle.h"
#include "GameFramework/Character.h"
#include "../../Characters/Enemies/BaseEnemy.h"
#include "../../Characters/Enemies/FighterEnemyAIController.h"
#include "../../Actors/EnemyManager.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UDecChoosePositionOutsideOfCircle::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter* character = OwnerComp.GetAIOwner()->GetCharacter();
	AFighterEnemyAIController* controller = Cast<AFighterEnemyAIController>(OwnerComp.GetAIOwner());
	if (character->IsValidLowLevelFast() && controller->IsValidLowLevelFast())
	{
		ABaseEnemy* enemy = Cast<ABaseEnemy>(character);
		if (enemy->IsValidLowLevelFast() && enemy->GetEnemyManager()->IsValidLowLevelFast())
		{
			FVector location;
			enemy->GetEnemyManager()->ChooseLocationOutsideCircle(enemy, location);
			controller->GetBlackBoard()->SetValueAsVector(controller->PointOutsideCircleKey, location);
			return true;
		}
	}
	return false;
}
