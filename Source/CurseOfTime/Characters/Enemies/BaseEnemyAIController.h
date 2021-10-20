// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class CURSEOFTIME_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

		UBehaviorTreeComponent* BehaviorTree;

		UBlackboardComponent* BlackBoard;
		void OnPossess(APawn* InPawn);
public:
	ABaseEnemyAIController();
	FORCEINLINE UBlackboardComponent* GetBlackBoard()const { return Blackboard; }
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorTree()const { return BehaviorTree; }


	UFUNCTION()
		void PlayerSensed();
	UFUNCTION()
		void Reset();

	FName PlayerKey = "Player";
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UBehaviorTree* BehaviorTreeAsset;

private: 
	APawn* Player;
};
