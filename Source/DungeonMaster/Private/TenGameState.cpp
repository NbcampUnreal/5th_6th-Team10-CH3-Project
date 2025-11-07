// Fill out your copyright notice in the Description page of Project Settings.


#include "TenGameState.h"
#include "Kismet/GameplayStatics.h"
#include "EnemySpawnVolume.h"
#include "ClearPortal.h"
#include "DungeonFinishPortal.h"
#include "TenGameInstance.h"
#include "TenPlayerController.h"
#include "TenCharacter.h"
#include "TenEnemyCharacter.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ATenGameState::ATenGameState()
    :MainMenuWidgetClass(nullptr),
    MainMenuWidgetInstance(nullptr)
{
    AliveEnemyCount = 0;
    SpawnedEnemyCount = 0;    
    TotalGoldWidget = nullptr;   // 변경: 위젯 초기화
}

void ATenGameState::BeginPlay()
{
	Super::BeginPlay();

	UpdateHUD();
    //골드 UI
    if (TotalGoldWidgetClass)
    {
        TotalGoldWidget = CreateWidget<UUserWidget>(GetWorld(), TotalGoldWidgetClass);
        if (TotalGoldWidget)
        {
            TotalGoldWidget->AddToViewport(1);
            UpdateTotalGoldUI(); // UI 초기 갱신
            UE_LOG(LogTemp, Warning, TEXT("[UI] TotalGoldWidget Created"));
        }
    }
    // 게임인스턴스 정보를 통해 '던전 진행중'인지 확인
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UTenGameInstance* TenGameInstance = Cast<UTenGameInstance>(GameInstance);
        if (TenGameInstance)
        {
            // 일반 던전 진행중이면 StageStart 호출
            if (TenGameInstance->CurrentStage < TenGameInstance->StageRepeats)
            {
                StageStart();
                UE_LOG(LogTemp, Warning, TEXT("Stage Start!"));
                
            }
            if (TenGameInstance->IsBossBattle == true)
            {
                BossBattle();
            }
        }
    }
    // 현재 레벨 정보 불러오기
    FString LevelName = GetWorld()->GetMapName();
    LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
    // MainUILevel 일 때, 메뉴 UI 생성
    if (LevelName == TEXT("MainUILevel")) 
    {
        MainUI();
    }
}

void ATenGameState::AddGold(int32 Amount)
{    
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UTenGameInstance* TenGameInstance = Cast<UTenGameInstance>(GameInstance);
        if (TenGameInstance)
        {   //골드 증가
            TenGameInstance->AddGold(Amount);
            //UI갱신 추가
            UpdateTotalGoldUI();
            // 디버그 로그 확인용
            UE_LOG(LogTemp, Warning, TEXT("[Gold] +%d (Total: %d)"),
                Amount, TenGameInstance->TotalGold);
        }
    }    
}

void ATenGameState::DungeonStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Dungeon Start!"));

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UTenGameInstance* TenGameInstance = Cast<UTenGameInstance>(GameInstance);
        if (TenGameInstance)
        {
            TenGameInstance->SetStageRepeats(5);
            TenGameInstance->SetCurrentStage(0);
        }
    }  

	CallRandomLevel();
}

void ATenGameState::CallRandomLevel()
{
    // 1~3 랜덤 스테이지 선택
    int32 RandomLevel = FMath::RandRange(1, 3);
    UE_LOG(LogTemp, Warning, TEXT("Selected Level %d"), RandomLevel);
    // 레벨이름 문자열 생성
    FString LevelName = FString::Printf(TEXT("Test_DungeonType%d"), RandomLevel);
    // 레벨 불러오기
    UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}

void ATenGameState::StageStart()
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UTenGameInstance* TenGameInstance = Cast<UTenGameInstance>(GameInstance);
        if (TenGameInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("Stage %d Start!"), TenGameInstance->CurrentStage + 1);
        }
    }
    
    // 적 스폰    
    TArray<AActor*> FoundVolumes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnVolume::StaticClass(), FoundVolumes);

    SpawnedEnemyCount = 5;
    AliveEnemyCount = 0;

    for (int32 i = 0; i < SpawnedEnemyCount; i++)
    {
        if (FoundVolumes.Num() > 0)
        {
            int32 RandomIndex = FMath::RandRange(0, FoundVolumes.Num() - 1);
            AEnemySpawnVolume* SpawnVolume = Cast<AEnemySpawnVolume>(FoundVolumes[RandomIndex]);
            if (SpawnVolume)
            {
                // 1. 적AI 랜덤 스폰
                AActor* SpawnedActor = SpawnVolume->SpawnRandomEnemy();
                // 2. 스폰된 액터를 AMonster 타입으로 캐스팅 (델리게이트)
                ATenEnemyCharacter* SpawnedMonster = Cast<ATenEnemyCharacter>(SpawnedActor);
                if (SpawnedMonster)
                {   //**델리게이트 중복 방지 코드 추가
                    if (!SpawnedMonster->OnEnemyKilled.IsAlreadyBound(this, &ATenGameState::OnKillEnemy))
                    {// 3. 몬스터의 델리게이트에 GameState의 함수를 바인딩(등록)
                        SpawnedMonster->OnEnemyKilled.AddDynamic(this, &ATenGameState::OnKillEnemy);
                    }// 현재 적 AI 카운트 증가
                    AliveEnemyCount++;
                }                            
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("%d Enemies Spawned"), AliveEnemyCount);
}

void ATenGameState::OnKillEnemy(int32 Gold)
{
    AddGold(Gold);  //골드 반영
    AliveEnemyCount--;
    UE_LOG(LogTemp, Warning, TEXT("AliveEnemyCount : %d "), AliveEnemyCount);

    if (AliveEnemyCount <= 0)
    {
        StageClear();
    }
}

void ATenGameState::StageClear()
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UTenGameInstance* TenGameInstance = Cast<UTenGameInstance>(GameInstance);
        if (TenGameInstance)
        {
            if (TenGameInstance->IsBossBattle == false)
            {
                // Clear 포탈 생성
                if (ClearPortalClass)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Stage %d Cleared!"), TenGameInstance->CurrentStage + 1);

                    FVector SpawnLocation(0, 0, 100); // 적절한 위치로 조정 가능
                    GetWorld()->SpawnActor<AClearPortal>(ClearPortalClass, SpawnLocation, FRotator::ZeroRotator);
                }
            }
            else
            {
                if (DungeonFinishPortalClass)
                {
                    FVector SpawnLocation(0, 0, 100); // 적절한 위치로 조정 가능
                    GetWorld()->SpawnActor<ADungeonFinishPortal>(DungeonFinishPortalClass, SpawnLocation, FRotator::ZeroRotator);
                }
            }
        }
    }    
}

// 클리어 후 포탈에 닿으면
void ATenGameState::OnClearPortalTouched()
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UTenGameInstance* TenGameInstance = Cast<UTenGameInstance>(GameInstance);
        if (TenGameInstance)
        {
            TenGameInstance->AddCurrentStage(1);

            if (TenGameInstance->CurrentStage < TenGameInstance->StageRepeats)
            {
                CallRandomLevel();
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("All Stages Cleared!"));
                // 클리어 맵으로 이동
                UGameplayStatics::OpenLevel(GetWorld(), FName("Test_DungeonClearLevel"));
            }
        }
    }    
}

void ATenGameState::ToMainLevel()
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UTenGameInstance* TenGameInstance = Cast<UTenGameInstance>(GameInstance);
        if (TenGameInstance)
        {
            TenGameInstance->SetStageRepeats(0);
            TenGameInstance->SetCurrentStage(0);
            TenGameInstance->BossBattle(false);
        }
    }
    // 클리어레벨에서 메인레벨로
    UGameplayStatics::OpenLevel(GetWorld(), FName("Test_MainLevel"));
}

void ATenGameState::PlayerDeath()
{
    // 페널티 추가 필요

    // 사망 시 UI 

    // 메인 레벨로    
    ToMainLevel();
}

void ATenGameState::BossStart()
{
    UE_LOG(LogTemp, Warning, TEXT("Boss Start!"));
    // 던전에서 보스로 갈 때 던전정보 초기화 (메인레벨에서 보스 돌입시 필요x)
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UTenGameInstance* TenGameInstance = Cast<UTenGameInstance>(GameInstance);
        if (TenGameInstance)
        {
            TenGameInstance->SetStageRepeats(0);
            TenGameInstance->SetCurrentStage(0);
            TenGameInstance->BossBattle(true);
        }
    }
    // 보스전투 레벨로 이동
    UGameplayStatics::OpenLevel(GetWorld(), FName("Test_BossLevel"));
}

void ATenGameState::BossBattle()
{
    SpawnedEnemyCount = 1;
    AliveEnemyCount = 1;
}


void ATenGameState::UpdateHUD()
{
    // 이전 과제에서 그대로 가져왔습니다.
    /*
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ATenPlayerController* TenPlayerController = Cast<ATenPlayerController>(PlayerController);
		{
			if (UUserWidget* HUDWidget = TenPlayerController->GetHUDWidget())
			{				
				// 골드
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Gold"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UTenGameInstance* TenGameInstance = Cast<UTenGameInstance>(GameInstance);
						if (TenGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), TenGameInstance->Gold)));
						}
					}
				}
				// 스테이지
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevel + 1)));
				}
				// 체력
				if (UTextBlock* HPText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HP"))))
				{
					float health = 1;
					// 1. PlayerController가 소유하고 있는 Pawn(캐릭터)을 가져옵니다.
					if (APawn* PlayerPawn = PlayerController->GetPawn())
					{
						// 2. Pawn을 TenCharacter 클래스로 캐스팅합니다.
						ATenCharacter* TenCharacter = Cast<ATenCharacter>(PlayerPawn);

						if (TenCharacter)
						{
							// 3. ATenCharacter의 GetHealth() 함수를 호출하여 실제 체력을 가져옵니다.
							health = TenCharacter->GetHealth();
						}
					}
					HPText->SetText(FText::FromString(FString::Printf(TEXT("HP: %.1f"), health)));
				}
			}
		}
	}
    */
}

void ATenGameState::MainUI()
{
    // 1. MainMenuWidgetClass가 할당되었는지 확인
    if (MainMenuWidgetClass)
    {
        MainMenuWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport();

            // PlayerController를 가져옴 (널 체크 포함)
            if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
            {
                // 2. ATenPlayerController로 캐스팅하고 널 체크 수행
                if (ATenPlayerController* TenPlayerController = Cast<ATenPlayerController>(PlayerController))
                {
                    TenPlayerController->SetInputMode(FInputModeUIOnly());
                    TenPlayerController->bShowMouseCursor = true;
                }
                // (선택) ATenPlayerController 캐스팅에 실패했을 경우 APlayerController를 사용한 기본 설정
                else
                {
                    PlayerController->SetInputMode(FInputModeUIOnly());
                    PlayerController->bShowMouseCursor = true;
                }
            }
        }
    }
}

void ATenGameState::UpdateTotalGoldUI()
{
    // 골드 수치 UI 업데이트
    if (!TotalGoldWidget) return;

    if (UTextBlock* GoldText = Cast<UTextBlock>(TotalGoldWidget->GetWidgetFromName(TEXT("TotalGoldText"))))
    {
        if (UGameInstance* GameInstance = GetGameInstance())
        {
            if (UTenGameInstance* TenGameInstance = Cast<UTenGameInstance>(GameInstance))
            {
                GoldText->SetText(FText::FromString(FString::Printf(TEXT("Gold: %d"), TenGameInstance->TotalGold)));
            }
        }
    }
}