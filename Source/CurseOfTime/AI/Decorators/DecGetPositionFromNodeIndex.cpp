// Fill out your copyright notice in the Description page of Project Settings.


#include "DecGetPositionFromNodeIndex.h"
#include "GameFramework/Character.h"
#include "../../Characters/Enemies/FighterEnemyAIController.h"
#include "../../Characters/Enemies/BaseEnemy.h"
#include "../../Actors/EnemyManager.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UDecGetPositionFromNodeIndex::CalculateRawConditionValue
(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// getting both the character and cast the controller to the base controller
	ACharacter* character = OwnerComp.GetAIOwner()->GetCharacter();
	AFighterEnemyAIController* controller = Cast<AFighterEnemyAIController>(OwnerComp.GetAIOwner());
	if (character->IsValidLowLevelFast() && controller->IsValidLowLevelFast())
	{
		ABaseEnemy* enemy = Cast<ABaseEnemy>(character);
		if (enemy->IsValidLowLevelFast() && enemy->GetEnemyManager()->IsValidLowLevelFast())
		{
			// getting the node id if possible 
			int nodeIndex = controller->GetBlackBoard()->GetValueAsInt(controller->NodeIndexKey);
			FVector pos; 
			if (enemy->GetEnemyManager()->GetPositionFromNodeIndex(nodeIndex, pos))
			{
				controller->GetBlackBoard()->SetValueAsVector(controller->NodePositionKey, pos);
				return true;
			}
		}
	}
	return false;
}
