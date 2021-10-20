// Copyright Epic Games, Inc. All Rights Reserved.
// same file as the play animation varient but then with montages
// so it can play anim events what doesn't happen with play_animation


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTPlayMontage.generated.h"

/**
 * 
 */
UCLASS()
class CURSEOFTIME_API UBTPlayMontage : public UBTTaskNode
{
		GENERATED_UCLASS_BODY()

		/** Animation asset to play. Note that it needs to match the skeleton of pawn this BT is controlling */
		UPROPERTY(Category = Node, EditAnywhere)
			UAnimMontage* AnimationToPlay;

	UPROPERTY(Category = Node, EditAnywhere)
		uint32 bLooping : 1;

	/** if true the task will just trigger the animation and instantly finish. Fire and Forget. */
	UPROPERTY(Category = Node, EditAnywhere)
		uint32 bNonBlocking : 1;

	UPROPERTY()
		UBehaviorTreeComponent* MyOwnerComp;

	UPROPERTY()
		USkeletalMeshComponent* CachedSkelMesh;

	EAnimationMode::Type PreviousAnimationMode;

	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	void OnAnimationTimerDone();

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

protected:
	void CleanUp(UBehaviorTreeComponent& OwnerComp);

public: 
	float AnimPlayRate = 1;
};
