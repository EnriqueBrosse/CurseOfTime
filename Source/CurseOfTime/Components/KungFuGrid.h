// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "KungFuGrid.generated.h"

USTRUCT(BlueprintType)
struct FKungFuNode
{
	GENERATED_USTRUCT_BODY();
public:
	bool bIsOccupied;
	int index;
	FVector Pos;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CURSEOFTIME_API UKungFuGrid : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKungFuGrid();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	float GetApproachCircleRadius()const { return ApproachCircleRadius; }
	float GetAttackCircleRadius()const { return AttackCircleRadius; }
	int GetGridCapacity()const { return GridCapacity; }
	int GetAttackCapacity()const { return AttackCapacity; }

	bool GetClosestNode(const FVector& pos, int& nodeIndex);

	bool GetPositionFromNodeIndex(int nodeIndex, FVector& position);

	void SetNodeUnoccupied(int nodeindex);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private: 
	void UpdateNodePositions();


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float ApproachCircleRadius;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float AttackCircleRadius;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int GridCapacity;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int AttackCapacity;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		bool bDrawDebug = true;

	UPROPERTY()
		TArray<FKungFuNode> Nodes;


	AActor* Owner;

		
};
