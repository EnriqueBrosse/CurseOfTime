// Fill out your copyright notice in the Description page of Project Settings.


#include "BTAttack.h"
#include "../../Characters/Enemies/FighterEnemy.h"
#include "../../Actors/MeleeWeapon.h"
#include "../../Components/HealthComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	AFighterEnemy* Enemy = Cast<AFighterEnemy>(OwnerComp.GetAIOwner()->GetCharacter());
	EBTNodeResult::Type result; 
	// checking if it has an animation to play
	if (!AnimationToPlay)
	{
		const FAttacks& currentAttack = Enemy->GetCurrentAttack();
		AnimationToPlay = currentAttack.Montage;
		AnimPlayRate = currentAttack.BaseAttackSpeed;
		bLooping = false;
	}

	result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (result == EBTNodeResult::Type::Succeeded)
	{
		AnimationToPlay = nullptr;
	}
	return result;
}



void UBTAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (HealthComponent && HealthComponent->GetIsDead())
	{
		Super::AbortTask(OwnerComp, NodeMemory);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}