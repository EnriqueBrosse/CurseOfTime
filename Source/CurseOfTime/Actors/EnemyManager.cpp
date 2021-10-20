// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "../Characters/Enemies/BaseEnemy.h"
#include "../Characters/Enemies/FighterEnemy.h"
#include "../Characters/Enemies/BaseEnemyAIController.h"
#include "../Components/KungFuGrid.h"
#include "MeleeWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Characters/Enemies/FighterEnemyAIController.h"


// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	KungFuGrid = Player->FindComponentByClass<UKungFuGrid>();

}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ABaseEnemy* AEnemyManager::GetEnemy(TSubclassOf<ABaseEnemy> enemyType)
{
	if (!EnemyPool.Contains(enemyType))
	{
		EnemyPool.Add(enemyType);
	}

	for (ABaseEnemy* enemy : EnemyPool[enemyType])
	{
		// the enemy has to be dead and can't be occupied by a spawner
		if (!enemy->GetIsActive() && !enemy->GetIsOccupied())
		{
			enemy->SetIsOccupied(true);
			enemy->SetIsActive(true);
			return enemy;
		}
	}

	// if there are no enemies on non active and not occupied make a new one and send that one 
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABaseEnemy* enemy = GetWorld()->SpawnActor<ABaseEnemy>(enemyType, spawnParameters);
	enemy->SetIsOccupied(true);
	enemy->SetIsActive(true);
	enemy->SetEnemyManager(this);
	EnemyPool[enemyType].Add(enemy);

	return enemy;
}


bool AEnemyManager::RequestAccesToApproachCircle(AFighterEnemy* enemy)
{
	if (EnemiesInApproachCircle.Find(enemy) != INDEX_NONE)
	{
		return true;
	}

	// looking if the enemy is outside of the circle 
	const float distanceSquared = FVector::DistSquaredXY(enemy->GetActorLocation(), KungFuGrid->GetComponentLocation());
	const float squareApproachCircle{ FMath::Square(KungFuGrid->GetApproachCircleRadius()) };
	if (distanceSquared < squareApproachCircle)
	{
		// but if the enemy is inside i want to know if he was attacking or not (check the attacking cicle)
		const float squareAttackingCircle{FMath::Square(KungFuGrid->GetAttackCircleRadius())};
		if (distanceSquared > squareAttackingCircle)
		{
			return false;
		}
	}

	if (CurrentGridCapacity + enemy->GetGridWeight() > KungFuGrid->GetGridCapacity())
	{
		return false;
	}
	int nodeIndex = -1;
	// node will be filled in here 
	if (!KungFuGrid->GetClosestNode(enemy->GetActorLocation(), nodeIndex))
	{
		return false;
	}
	AFighterEnemyAIController* baseController = Cast<AFighterEnemyAIController>(enemy->GetController());
	if (baseController->IsValidLowLevelFast())
	{
		baseController->GetBlackBoard()->SetValueAsInt(baseController->NodeIndexKey, nodeIndex);
	}
	CurrentGridCapacity += enemy->GetGridWeight();
	EnemiesInApproachCircle.Add(enemy);
	return true;
}

bool AEnemyManager::RequestAttack(AFighterEnemy* enemy, FAttacks& returnAttack)
{
	// let them attack 
	const float maxDistance{ 20.f};
	AFighterEnemyAIController* controller = Cast<AFighterEnemyAIController>(enemy->GetController());
	if (!controller->IsValidLowLevelFast())
	{
		return false;
	}

	FVector nodeMiddlePos = controller->GetBlackBoard()->GetValueAsVector(controller->NodePositionKey);

	if (FVector::DistSquaredXY(enemy->GetActorLocation(), nodeMiddlePos) < FMath::Square(maxDistance))
	{
		RemoveEnemyFromCircle(enemy);
		return false;
	}

	// getting it's weapon and also it's attacks
	AMeleeWeapon* meleeWeapon = enemy->GetMeleeWeapon();
	//if (!meleeWeapon->IsValidLowLevelFast())
	//{
	//	return false;
	//}

	TArray<FAttacks> attacks = meleeWeapon->GetLightAttacks();
	for (const FAttacks& attack : meleeWeapon->GetHeavyAttacks())
	{
		attacks.Add(attack);
	}
	TArray<FAttacks> possibleAttacks;
	const int attackCapacity = KungFuGrid->GetAttackCapacity();
	for (const FAttacks& attack : attacks)
	{
		if (attack.AttackWeight + CurrentAttackCapacity <= attackCapacity)
		{
			possibleAttacks.Add(attack);
		}
	}

	if (possibleAttacks.Num() == 0)
	{
		RemoveEnemyFromCircle(enemy);
		return false;
	}

	returnAttack = possibleAttacks[FMath::RandRange(0, possibleAttacks.Num() - 1)];
	CurrentAttackCapacity += returnAttack.AttackWeight;
	CurrentAttacks.Add(enemy, returnAttack);
	return true;
}

bool AEnemyManager::GetPositionFromNodeIndex(int nodeIndex, FVector& position)
{
	return KungFuGrid->GetPositionFromNodeIndex(nodeIndex, position);
}

void AEnemyManager::RemoveEnemyFromCircle(AFighterEnemy* enemy)
{
	const int32 enemyidx = EnemiesInApproachCircle.Find(enemy);
	if (enemyidx == INDEX_NONE)
	{
		return;
	}
	EnemiesInApproachCircle.Remove(enemy);
	CurrentGridCapacity -= enemy->GetGridWeight();
	AFighterEnemyAIController* controller = Cast<AFighterEnemyAIController>(enemy->Controller);
	if (controller)
	{
		int nodeIndex = controller->GetBlackBoard()->GetValueAsInt(controller->NodeIndexKey);
		KungFuGrid->SetNodeUnoccupied(nodeIndex);
		nodeIndex = INDEX_NONE;
		controller->GetBlackBoard()->SetValueAsInt(controller->NodeIndexKey, nodeIndex);
	}
	FAttacks* attack = CurrentAttacks.Find(enemy);
	if (attack)
	{
		CurrentAttacks.Remove(enemy);
		CurrentAttackCapacity -= attack->AttackWeight;
	}
}

void AEnemyManager::ChooseLocationOutsideCircle(ABaseEnemy* enemy, FVector& location)
{
	const FVector& backwards = -enemy->GetActorForwardVector();
	location = enemy->GetActorLocation() + (KungFuGrid->GetApproachCircleRadius() * backwards);
}