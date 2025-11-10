// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandWithinSpawn.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMASTER_API UBTTask_RandWithinSpawn : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RandWithinSpawn();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector LocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, Category = "Patrol", meta = (ClampMin = "100.0"))
	float PatrolRadius = 300.0f;
};
