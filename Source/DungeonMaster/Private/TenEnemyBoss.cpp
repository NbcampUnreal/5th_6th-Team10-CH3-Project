// Fill out your copyright notice in the Description page of Project Settings.


#include "TenEnemyBoss.h"
#include "Kismet/GameplayStatics.h"

ATenEnemyBoss::ATenEnemyBoss()
{
}

void ATenEnemyBoss::SkillFirePillar(FVector Location)
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
            FTimerDelegate TimerDel;
            TimerDel.BindUFunction(this, FName("SpawnCircle"), Location);
            GetWorld()->GetTimerManager().SetTimer(
                AnimEnd,
                TimerDel,
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

void ATenEnemyBoss::SpawnCircle(FVector Location)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    FRotator SpawnRotation;
    FVector  SpawnLocation = Location;
    SpawnLocation.Z = 0.0f;
    GetWorld()->SpawnActor<AActor>(SkillActorClass, SpawnLocation, SpawnRotation, SpawnParams);

    FTimerHandle CircleEnd;
    FTimerDelegate TimerDel;
    TimerDel.BindUFunction(this, FName("DamageFirePillar"), Location);
    GetWorld()->GetTimerManager().SetTimer(
        CircleEnd,
        TimerDel,
        3.0f,
        false
    );
}

void ATenEnemyBoss::DamageFirePillar(FVector Location)
{
    UGameplayStatics::ApplyRadialDamage(
        this,
        Damage * 2.0f,
        Location,
        100.0f, // 불기둥 반경
        UDamageType::StaticClass(),
        TArray<AActor*>{},
        this,
        GetInstigatorController(),
        true
    );
    UE_LOG(LogTemp, Warning, TEXT("[DM] DamageFirePillar"));
}
