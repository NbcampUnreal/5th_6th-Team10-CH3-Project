// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SkillFirePillar.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMASTER_API UBTTask_SkillFirePillar : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_SkillFirePillar();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
