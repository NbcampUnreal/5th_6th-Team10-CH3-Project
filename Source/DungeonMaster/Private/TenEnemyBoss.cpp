// Fill out your copyright notice in the Description page of Project Settings.


#include "TenEnemyBoss.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TenAIController.h"

ATenEnemyBoss::ATenEnemyBoss()
{
    Gold = 200;
}

void ATenEnemyBoss::SkillFirePillar()
{
    //애님 몽타주 재생(공격)
    if (AttackMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage))
        {
            AnimInstance->Montage_Play(AttackMontage);
            float AnimLength = 1.0f;

            FTimerHandle AnimEnd;
            GetWorld()->GetTimerManager().SetTimer(
                AnimEnd,
                this,
                &ATenEnemyBoss::SpawnCircle,
                AnimLength,
                false
            );
            UE_LOG(LogTemp, Warning, TEXT("[DM] SkillFirePillar"));
        }
    }
}

void ATenEnemyBoss::BeginPlay()
{
	Super::BeginPlay();
}

void ATenEnemyBoss::SpawnCircle()
{
    ATenAIController* AIController = Cast<ATenAIController>(GetController());
    if (AIController)
    {
        AActor* TargetActor = Cast<AActor>(AIController->GetBlackboardComp()->GetValueAsObject(TEXT("TargetActor")));
        if (TargetActor == nullptr) return;

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        FRotator SpawnRotation;
        FVector  SpawnLocation = TargetActor->GetActorLocation();
        SpawnLocation.Z = 0.01f;
        GetWorld()->SpawnActor<AActor>(SkillActorClass, SpawnLocation, SpawnRotation, SpawnParams);

        FTimerHandle CircleEnd;
        FTimerDelegate TimerDel;
        TimerDel.BindUFunction(this, FName("DamageFirePillar"), SpawnLocation);
        GetWorld()->GetTimerManager().SetTimer(
            CircleEnd,
            TimerDel,
            2.0f,
            false
        );
    }
}

void ATenEnemyBoss::DamageFirePillar(FVector Location)
{
    UGameplayStatics::ApplyRadialDamage(
        this,
        Damage * 2.0f,
        Location,
        480.0f, // 불기둥 반경
        UDamageType::StaticClass(),
        TArray<AActor*>{},
        this,
        GetInstigatorController(),
        true
    );
    UE_LOG(LogTemp, Warning, TEXT("[DM] DamageFirePillar"));
}
