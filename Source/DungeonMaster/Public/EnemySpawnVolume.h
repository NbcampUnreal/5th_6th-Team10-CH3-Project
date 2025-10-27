// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnRow.h"
#include "EnemySpawnVolume.generated.h"

class UBoxComponent;


UCLASS()
class DUNGEONMASTER_API AEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnVolume();

    UFUNCTION(BlueprintCallable, Category = "EnemySpawning")
    AActor* SpawnRandomEnemy();
  
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemySpawning")
    USceneComponent* Scene;
    // 스폰 영역을 담당할 박스 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemySpawning")
    UBoxComponent* SpawningBox;
    // 적 스폰 확률 데이터테이블
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemySpawning")
    UDataTable* EnemyDataTable;

    // 스폰 볼륨 내부에서 무작위 좌표를 얻어오는 함수
    FVector GetRandomPointInVolume() const;
    // 특정 아이템 클래스를 스폰하는 함수
    AActor* SpawnEnemy(TSubclassOf<AActor> EnemyClass);

    FEnemySpawnRow* GetRandomEnemy() const;

};
