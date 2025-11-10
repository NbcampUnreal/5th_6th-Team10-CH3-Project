// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SkillFirePillar.h"
#include "AIController.h"
#include "TenEnemyBoss.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SkillFirePillar::UBTTask_SkillFirePillar()
{
	NodeName = TEXT("Skill FirePillar");
}

EBTNodeResult::Type UBTTask_SkillFirePillar::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) { return EBTNodeResult::Failed; }

    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn) { return EBTNodeResult::Failed; }

    ATenEnemyBoss* Monster = Cast<ATenEnemyBoss>(ControlledPawn);
    if (!Monster) { return EBTNodeResult::Failed; }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
        Monster->SkillFirePillar();
        UE_LOG(LogTemp, Warning, TEXT("[DM] FirePillar!!"));
    }


    return EBTNodeResult::Succeeded;
}
