// Fill out your copyright notice in the Description page of Project Settings.


#include "KungFuGrid.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UKungFuGrid::UKungFuGrid()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKungFuGrid::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = GetOwner();


	for (size_t i = 0; i < 8; i++)
	{
		FKungFuNode node{};
		node.index = i;
		Nodes.Add(node);
	}
	UpdateNodePositions();

}


// Called every frame
void UKungFuGrid::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	UpdateNodePositions();
	if (bDrawDebug)
	{
		UWorld* world = GetWorld();
		FVector location = GetComponentLocation();
		FVector end = location + FVector(0, 0, 25);
		DrawDebugCylinder(world, location, end, ApproachCircleRadius, 8, FColor::Black, false, 0, 0, 2);
		DrawDebugCylinder(world, location, end, AttackCircleRadius, 8, FColor::Red, false, 0, 0, 2);
		for (size_t i = 0; i < Nodes.Num(); i++)
		{
			DrawDebugSphere(world, Nodes[i].Pos, 10, 8, FColor::Blue, false, 0, 0, 2);
		}
	}
}

void UKungFuGrid::UpdateNodePositions()
{

	const int size = Nodes.Num();
	const float anglePerNode = 2 * float(PI) / float(size);
	// the approachCircleRadius should always be bigger then the circleRadius
	const float differenceBetweenRadiuses = ApproachCircleRadius - AttackCircleRadius;
	const float radius = AttackCircleRadius + (differenceBetweenRadiuses / 2.f);
	FVector Middlepos = GetComponentLocation();


	for (int i = 0; i < size; i++)
	{
		Nodes[i].Pos.X = Middlepos.X + radius * cosf(i * anglePerNode);
		Nodes[i].Pos.Y = Middlepos.Y + radius * sinf(i * anglePerNode);
		Nodes[i].Pos.Z = Middlepos.Z;
	}

}

void UKungFuGrid::SetNodeUnoccupied(int nodeIndex)
{
	if (nodeIndex >= Nodes.Num() || nodeIndex == INDEX_NONE)
	{
		return;
	}
	Nodes[nodeIndex].bIsOccupied = false;
}

bool UKungFuGrid::GetClosestNode(const FVector& pos, int& nodeIndex)
{
	TArray<FKungFuNode> UnoccupiedNodes; 
	for (const FKungFuNode& node : Nodes)
	{
		if (!node.bIsOccupied)
		{
			UnoccupiedNodes.Add(node);
		}
	}	
	if (UnoccupiedNodes.Num() == 0)
	{
		return false;
	}
	FKungFuNode closestNode; 
	float closestDistance = FLT_MAX;

	for (const FKungFuNode& node : UnoccupiedNodes)
	{
		const float distance = FVector::DistSquaredXY(node.Pos, pos);
		if (distance < closestDistance)
		{
			closestDistance = distance;
			closestNode = node;
		}
	}
	Nodes[closestNode.index].bIsOccupied = true;
	nodeIndex = closestNode.index;
	return true;
}

bool UKungFuGrid::GetPositionFromNodeIndex(int nodeIndex, FVector& position)
{
	if (nodeIndex >= Nodes.Num() || nodeIndex == INDEX_NONE)
	{
		return false;
	}
	position = Nodes[nodeIndex].Pos;
	return true;
}
