// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TenGameState.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMASTER_API ATenGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ATenGameState();

	virtual void BeginPlay() override;
	
	// 현재 레벨에서 스폰된 적의 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 SpawnedEnemyCount;
	// 남아있는 적의 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 AliveEnemyCount;	
	// 실제 레벨 맵 이름 배열. 여기 있는 인덱스를 차례대로 연동
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage")
	TArray<FName> LevelMapNames;
	// Clear 액터 설정
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ClearPortalClass;
	// 메인메뉴 UI 호출
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainMenu")
	UUserWidget* MainMenuWidgetInstance;

	// 골드 추가 함수
	UFUNCTION(BlueprintCallable, Category = "Gold")
	void AddGold(int32 Amount);
	
	// 던전을 시작할 때
	UFUNCTION(Exec)
	void DungeonStart();	
	// 랜덤 레벨 불러오기
	void CallRandomLevel();
	// 스테이지 시작
	void StageStart();
	// 스테이지 클리어
	void StageClear();	
	// 포탈에 닿으면 (다음 스테이지) 
	UFUNCTION(Exec)
	void OnClearPortalTouched();
	// 메인 레벨로 (던전 클리어, 사망 등)
	void ToMainLevel();
	// 적 처치 시 호출
	UFUNCTION(Exec)
	void OnKillEnemy();
	// 사망 시
	void PlayerDeath();
	// 보스전투 시작할 때
	UFUNCTION(Exec)
	void BossStart();
	// HUD 업데이트
	void UpdateHUD();
	// MainUI 띄우기
	void MainUI();

};
