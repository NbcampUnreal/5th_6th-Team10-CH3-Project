// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RandWithinSpawn.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RandWithinSpawn::UBTTask_RandWithinSpawn()
{
	NodeName = TEXT("Find Random Location Within Spawn");

	LocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RandWithinSpawn, LocationKey));
}

EBTNodeResult::Type UBTTask_RandWithinSpawn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 1단계: 필요한 것들 가져오기
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* MyPawn = AIController->GetPawn();
    if (!MyPawn) return EBTNodeResult::Failed;

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        SpawnLocation = BlackboardComp->GetValueAsVector("SpawnLocation");
    }

    // 2단계: 랜덤 위치 찾기
    FNavLocation RandomLocation;
    bool bFound = NavSystem->GetRandomReachablePointInRadius(
        SpawnLocation,
        PatrolRadius,
        RandomLocation
    );

    // 3단계: 찾았으면 Blackboard에 저장
    if (bFound)
    {
        if (BlackboardComp)
        {
            BlackboardComp->SetValueAsVector(LocationKey.SelectedKeyName, RandomLocation.Location);
            UE_LOG(LogTemp, Log, TEXT("[FindRandom] 새로운 목적지: %s"), *RandomLocation.Location.ToString());
            return EBTNodeResult::Succeeded;  // 성공ㅋㅋ
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("[FindRandom] 갈 곳을 찾지 못했습니다"));
    return EBTNodeResult::Failed;  // 실패
}
