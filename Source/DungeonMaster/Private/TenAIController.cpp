// Fill out your copyright notice in the Description page of Project Settings.


#include "TenAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"

ATenAIController::ATenAIController()
{
	// BlackBoard
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard"));

	// Perception
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.0f;
	SightConfig->LoseSightRadius = 1500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SetMaxAge(5.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ATenAIController::StartBehaviorTree()
{
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
		UE_LOG(LogTemp, Warning, TEXT("[DM] Behavior Tree started"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[DM] Behavior Tree Asset not set!"));
	}
}

void ATenAIController::StopAI()
{
	if (UBrainComponent* BrainComp = GetBrainComponent())
	{
		BrainComp->StopLogic(TEXT("Monster Died"));
		UE_LOG(LogTemp, Warning, TEXT("[DM] AI Logic Stopped"));
	}

	// AI Perception 비활성화
	if (AIPerception)
	{
		AIPerception->Deactivate();
		AIPerception->OnPerceptionUpdated.RemoveAll(this);
		UE_LOG(LogTemp, Warning, TEXT("[DM] AI Perception Deactivated"));
	}

	UnPossess();
}

void ATenAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(TEXT("CanSeeTarget"), false);
		BlackboardComp->SetValueAsBool(TEXT("IsInvestigating"), false);
		UE_LOG(LogTemp, Warning, TEXT("[DM] Blackboard init success"));

		StartBehaviorTree();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[DM] Blackboard init fail"));
	}

	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(
			this,
			&ATenAIController::OnPerceptionUpdated
		);
		UE_LOG(LogTemp, Warning, TEXT("[DM] AIPerception Binding success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[DM] AIPerception Binding fail"));
	}
}

void ATenAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsVector(TEXT("SpawnLocation"), InPawn->GetActorLocation());
		}
	}
}

void ATenAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Actor != PlayerPawn || !BlackboardComp)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
		BlackboardComp->SetValueAsBool(TEXT("CanSeeTarget"), true);
		BlackboardComp->SetValueAsBool(TEXT("IsInvestigating"), false);
		UE_LOG(LogTemp, Warning, TEXT("Success : Target actor is %s"), *Actor->GetName());
	}
	else
	{
		BlackboardComp->SetValueAsBool(TEXT("CanSeeTarget"), false);
		BlackboardComp->SetValueAsBool(TEXT("IsInvestigating"), true);
		BlackboardComp->SetValueAsVector(TEXT("TargetLastKnownLocation"), Stimulus.StimulusLocation);
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
	}
}