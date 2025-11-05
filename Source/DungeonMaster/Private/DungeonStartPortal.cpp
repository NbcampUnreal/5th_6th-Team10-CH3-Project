// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonStartPortal.h"
#include "TenGameState.h"

ADungeonStartPortal::ADungeonStartPortal()
{
	PortalType = "DungeonStartPortal";
}

void ADungeonStartPortal::MoveLevel()
{
	if (ATenGameState* GameState = GetWorld()->GetGameState<ATenGameState>())
	{
		GameState->DungeonStart();
	}
}
