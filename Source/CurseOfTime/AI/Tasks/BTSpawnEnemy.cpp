// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSpawnEnemy.h"

#include "../../Characters/Enemies/SpawnerEnemy.h"
#include "../../Actors/MeleeWeapon.h"
#include "../../Components/HealthComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTSpawnEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!HealthComponent)
	{
		AActor* Owner = OwnerComp.GetAIOwner()->GetCharacter();
		if (Owner)
		{
			HealthComponent = Owner->FindComponentByClass<UHealthComponent>();
		}
	}
	// doing extra check because i could have filled it in the last frame 
	if (HealthComponent)
	{
		if (HealthComponent->GetIsDead())
		{
			return EBTNodeResult::Type::Failed;
		}
	}
	EBTNodeResult::Type result;

	// checking if it has an animation to play
	if (!AnimationToPlay)
	{
		ASpawnerEnemy* Enemy = Cast<ASpawnerEnemy>(OwnerComp.GetAIOwner()->GetCharacter());
		AnimationToPlay = Enemy->GetSpawnMontage();
		//const FAttacks& currentAttack = Enemy->GetCurrentAttack();
		//AnimationToPlay = currentAttack.Montage;
		AnimPlayRate = Enemy->GetAnimPlayRate();
		bLooping = false;
	}

	result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (result == EBTNodeResult::Type::Succeeded)
	{
		AnimationToPlay = nullptr;
	}
	return result;
}
