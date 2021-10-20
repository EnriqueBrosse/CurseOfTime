// Fill out your copyright notice in the Description page of Project Settings.


#include "DecRequestAttack.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "../../Characters/Enemies/FighterEnemy.h"
#include "../../Actors/EnemyManager.h"
#include "../../Actors/MeleeWeapon.h"


bool UDecRequestAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter* character = OwnerComp.GetAIOwner()->GetCharacter();
	if (character->IsValidLowLevelFast())
	{
		AFighterEnemy* enemy = Cast<AFighterEnemy>(character);
		if (enemy->IsValidLowLevelFast() && enemy->GetEnemyManager()->IsValidLowLevelFast())
		{
			int nodeIndex = 0;
			FAttacks attack{};

			if (enemy->GetEnemyManager()->RequestAttack(enemy, attack))
			{
				enemy->SetCurrentAttack(attack);
				return true;
			}
		}
	}
	return false;
}
