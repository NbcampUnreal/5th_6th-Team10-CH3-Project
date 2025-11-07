// Fill out your copyright notice in the Description page of Project Settings.


#include "TenBossAIController.h"
#include "Perception/AISenseConfig_Sight.h"

ATenBossAIController::ATenBossAIController()
{
	SightConfig->SightRadius = 3000.0f;
	SightConfig->LoseSightRadius = 3500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
}
