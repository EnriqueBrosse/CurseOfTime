// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerEnemy.h"
#include "../../Components/SpawnerComponent.h"

ASpawnerEnemy::ASpawnerEnemy()
	:ABaseEnemy()
{
	SpawnerComponent = CreateDefaultSubobject<USpawnerComponent>(TEXT("SpawnerComponent"));
	SpawnerComponent->SetupAttachment(RootComponent);
}