// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyAttack.h"
#include "AIController.h"
#include "TenEnemyCharacter.h"

UBTTask_EnemyAttack::UBTTask_EnemyAttack()
{
    NodeName = TEXT("Monster Attack");
}

EBTNodeResult::Type UBTTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) { return EBTNodeResult::Failed; }

    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn) { return EBTNodeResult::Failed; }

    ATenEnemyCharacter* Monster = Cast<ATenEnemyCharacter>(ControlledPawn);
    if (!Monster) { return EBTNodeResult::Failed; }

    Monster->MonsterAttack(); // 몬스터 공격 함수 호출

    return EBTNodeResult::Succeeded;
}
