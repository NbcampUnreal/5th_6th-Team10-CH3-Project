// Fill out your copyright notice in the Description page of Project Settings.


#include "BossPortal.h"
#include "TenGameState.h"

ABossPortal::ABossPortal()
{
	PortalType = "BossPortal";
}

void ABossPortal::MoveLevel()
{
	if (ATenGameState* GameState = GetWorld()->GetGameState<ATenGameState>())
	{
		GameState->BossStart();
	}
}
