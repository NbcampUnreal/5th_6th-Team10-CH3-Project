// Fill out your copyright notice in the Description page of Project Settings.


#include "TenEnemyCharacter.h"
#include "TenAIController.h"

// Sets default values
ATenEnemyCharacter::ATenEnemyCharacter()
{
	AIControllerClass = ATenAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ATenEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
