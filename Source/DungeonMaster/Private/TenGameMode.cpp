// Fill out your copyright notice in the Description page of Project Settings.


#include "TenGameMode.h"
#include "TenCharacter.h"
#include "TenPlayerController.h"
#include "TenGameState.h"

ATenGameMode::ATenGameMode()
{
	DefaultPawnClass = ATenCharacter::StaticClass();
	PlayerControllerClass = ATenPlayerController::StaticClass();
	GameStateClass = ATenGameState::StaticClass();
}