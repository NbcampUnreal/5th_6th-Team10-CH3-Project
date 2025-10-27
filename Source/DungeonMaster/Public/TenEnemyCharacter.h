// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TenEnemyCharacter.generated.h"

UCLASS()
class DUNGEONMASTER_API ATenEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATenEnemyCharacter();

protected:
	virtual void BeginPlay() override;
};
