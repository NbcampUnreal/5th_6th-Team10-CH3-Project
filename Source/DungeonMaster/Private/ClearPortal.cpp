// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearPortal.h"
#include "TenGameState.h"

AClearPortal::AClearPortal()
{
	PortalType = "ClearPortal";
}

void AClearPortal::MoveLevel()
{
	if (ATenGameState* GameState = GetWorld()->GetGameState<ATenGameState>())
	{
		GameState->OnClearPortalTouched();
	}
}
