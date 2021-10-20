// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAIController.h"
#include "Kismet/GameplayStatics.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

ABaseEnemyAIController::ABaseEnemyAIController()
{
	BehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BlackBoard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));
}

void ABaseEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!Player->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Error, TEXT("ABaseEnemyAIController::BeginPlay couldn't find player"));
		return;
	}
}

void ABaseEnemyAIController::PlayerSensed()
{
	Blackboard->SetValueAsObject(PlayerKey, Player);
	FTimerHandle timeHandle;
	// very small delay to reset the player sensing component didn't work
	//GetWorldTimerManager().SetTimer(timeHandle, this, &ABaseEnemyAIController::Reset, 0.001f, false);;
}

void ABaseEnemyAIController::Reset()
{
	Blackboard->SetValueAsObject(PlayerKey, nullptr);
}

void ABaseEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviorTreeAsset && BehaviorTreeAsset->BlackboardAsset)
	{
		Blackboard->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		BehaviorTree->StartTree(*BehaviorTreeAsset);
		Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (!Player->IsValidLowLevelFast())
		{
			return;
		}
		Blackboard->SetValueAsObject(PlayerKey, Player);
	}
}