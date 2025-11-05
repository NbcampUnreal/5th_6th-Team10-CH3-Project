// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonFinishPortal.h"
#include "TenGameState.h"

ADungeonFinishPortal::ADungeonFinishPortal()
{
	PortalType = "DungeonFinishPortal";
}

void ADungeonFinishPortal::MoveLevel()
{
	if (ATenGameState* GameState = GetWorld()->GetGameState<ATenGameState>())
	{
		GameState->ToMainLevel();
	}
}
