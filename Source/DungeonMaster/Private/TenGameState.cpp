// Fill out your copyright notice in the Description page of Project Settings.


#include "TenGameState.h"
#include "Kismet/GameplayStatics.h"
#include "EnemySpawnVolume.h"
#include "ClearPortal.h"
#include "TenGameInstance.h"
#include "TenPlayerController.h"
#include "TenCharacter.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ATenGameState::ATenGameState()
{
	Gold = 0;
    AliveEnemyCount = 0;
    SpawnedEnemyCount = 0;      
}

void ATenGameState::BeginPlay()
{
	Super::BeginPlay();

	UpdateHUD();

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UTenGameInstance* TenGameInstance = Cast<UTenGameInstance>(GameInstance);
        if (TenGameInstance)
        {
            if (TenGameInstance->CurrentStage < TenGameInstance->StageRepeats)
            {
                StageStart();
                UE_LOG(LogTemp, Warning, TEXT("Stage Start!"));
            }
        }
    }
}

int32 ATenGameState::GetGold() const
{
    return Gold;
}

void ATenGameState::AddGold(int32 Amount)
{
    
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UTenGameInstance* TenGameInstance = Cast<UTenGameInstance>(GameInstance);
        if (TenGameInstance)
        {
            TenGameInstance->AddGold(Amount);
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
            AEnemySpawnVolume* SpawnVolume = Cast<AEnemySpawnVolume>(FoundVolumes[0]);
            if (SpawnVolume)
            {
                // 적AI 랜덤 스폰
                AActor* SpawnedActor = SpawnVolume->SpawnRandomEnemy();
                // 현재 적 AI 카운트 증가
                AliveEnemyCount++;                
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("%d Enemies Spawned"), AliveEnemyCount);
}

void ATenGameState::OnKillEnemy()
{
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
            UE_LOG(LogTemp, Warning, TEXT("Stage %d Cleared!"), TenGameInstance->CurrentStage + 1);
        }
    }

    // Clear 포탈 생성
    if (ClearPortalClass)
    {
        FVector SpawnLocation(0, 0, 100); // 적절한 위치로 조정 가능
        GetWorld()->SpawnActor<AClearPortal>(ClearPortalClass, SpawnLocation, FRotator::ZeroRotator);
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