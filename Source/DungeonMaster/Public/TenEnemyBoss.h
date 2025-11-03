// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TenEnemyCharacter.h"
#include "TenEnemyBoss.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMASTER_API ATenEnemyBoss : public ATenEnemyCharacter
{
	GENERATED_BODY()
public:
	ATenEnemyBoss();
	void SkillFirePillar(FVector Location);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSubclassOf<AActor> SkillActorClass;

private:
	UFUNCTION()
	void SpawnCircle(FVector Location);

	UFUNCTION()
	void DamageFirePillar(FVector Location);
};
