// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TenGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMASTER_API UTenGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UTenGameInstance();

	// 현재 진행 중인 스테이지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 CurrentStage;
	// 던전 시작 시 전체 스테이지의 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameData")
	int32 StageRepeats;
	// 게임 전체 누적 골드
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalGold;
	// 보스전 여부
	bool IsBossBattle;

	

	// 골드 획득 함수
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddGold(int32 Amount);

	void AddCurrentStage(int8 amount);
	void SetStageRepeats(int32 amount);
	void SetCurrentStage(int32 amount);
	void BossBattle(bool EnterBoss);
};
