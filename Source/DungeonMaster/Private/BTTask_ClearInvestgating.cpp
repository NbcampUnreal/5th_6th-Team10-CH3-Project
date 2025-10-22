// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearInvestgating.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearInvestgating::UBTTask_ClearInvestgating()
{
	NodeName = TEXT("Clear Investigating Flag");
}

EBTNodeResult::Type UBTTask_ClearInvestgating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsBool("IsInvestigating", false);

        return EBTNodeResult::Succeeded;
    }
	
    return EBTNodeResult::Failed;
}
