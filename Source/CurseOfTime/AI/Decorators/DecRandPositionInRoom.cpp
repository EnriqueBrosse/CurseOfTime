// Fill out your copyright notice in the Description page of Project Settings.


#include "DecRandPositionInRoom.h"
#include "Kismet/GameplayStatics.h"
#include "../../Actors/LevelManager.h"
#include "../../Actors/RoomController.h"
#include "../../Characters/Enemies/SpawnerAIController.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"


#include "../../Characters/Enemies/SpawnerAIController.h"
#include "../../Characters/Enemies/BaseEnemy.h"
#include "../../Actors/EnemyManager.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "DrawDebugHelpers.h"


bool UDecRandPositionInRoom::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const
{
	TArray<AActor*> managers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelManager::StaticClass(), managers);
	if (managers.Num() == 0)
	{
		return false;
	}
	ALevelManager* levelMan = Cast<ALevelManager>(managers[0]);
	int levelId = levelMan->GetStreamingLevelId(OwnerComp.GetAIOwner()->GetCharacter()->GetActorLocation());
	// bad hotfix 
	if (levelId > 40)
	{
		levelId = 40;
	}

	TArray<AActor* > controllers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoomController::StaticClass(), controllers);
	for (size_t i = 0; i < controllers.Num(); i++)
	{
		ARoomController* tempController = Cast<ARoomController>(controllers[i]);
		if (tempController->GetStreamingLevelId() == levelId)
		{
			UBoxComponent* boxComp = tempController->GetLevelSizeBox(); 
			FVector returnPosition = UKismetMathLibrary::RandomPointInBoundingBox(boxComp->Bounds.Origin, boxComp->Bounds.BoxExtent);
			returnPosition.Z = OwnerComp.GetOwner()->GetActorLocation().Z;

			ACharacter* character = OwnerComp.GetAIOwner()->GetCharacter();
			ASpawnerAIController* controller = Cast<ASpawnerAIController>(OwnerComp.GetAIOwner());
			if (character->IsValidLowLevelFast() && controller->IsValidLowLevelFast())
			{
				DrawDebugBox(GetWorld(), returnPosition, FVector(10, 10, 10),FColor::Black,true);
				// saving the position
				controller->GetBlackBoard()->SetValueAsVector(controller->RandomPointInRoomKey, returnPosition);
			}
			return true;
		}
	}
	return false;
}
