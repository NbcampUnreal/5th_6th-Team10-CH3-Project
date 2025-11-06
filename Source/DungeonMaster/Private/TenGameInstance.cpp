// Fill out your copyright notice in the Description page of Project Settings.


#include "TenGameInstance.h"

UTenGameInstance::UTenGameInstance()
{
	TotalGold = 0;
	CurrentStage = 0;
	StageRepeats = 0;
	IsBossBattle = false;
}

void UTenGameInstance::AddGold(int32 Amount)
{
	TotalGold += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalGold);
}

void UTenGameInstance::AddCurrentStage(int8 amount)
{
	CurrentStage += amount;
}

void UTenGameInstance::SetStageRepeats(int32 amount)
{
	StageRepeats = amount;
}

void UTenGameInstance::SetCurrentStage(int32 amount)
{
	CurrentStage = amount;
}

void UTenGameInstance::BossBattle(bool EnterBoss)
{
	IsBossBattle = EnterBoss;
}

