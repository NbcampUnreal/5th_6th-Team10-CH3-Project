// Fill out your copyright notice in the Description page of Project Settings.


#include "TenEnemyCharacter.h"
#include "TenAIController.h"
#include "CombatComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATenEnemyCharacter::ATenEnemyCharacter()
	: MaxHealth(100.f),
	CurrentHealth(100.f),
	Damage(10.f),
	Range(100.f)
{
	AIControllerClass = ATenAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AttackMontage = nullptr;
	HitReactMontage = nullptr;
	DieMontage = nullptr;
	bIsDead = false;
}

float ATenEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead)
	{
		return DamageAmount;
	}

	float NewHealth = CurrentHealth - DamageAmount;
	SetCurrentHealth(NewHealth);
	
	//애님 몽타주 재생(피격)
	if (HitReactMontage && !bIsDead)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && !AnimInstance->Montage_IsPlaying(HitReactMontage))
		{
			AnimInstance->Montage_Play(HitReactMontage);
			UE_LOG(LogTemp, Warning, TEXT("[DM] Play HitReactMontage"));
		}
	}

	return DamageAmount;
}

void ATenEnemyCharacter::MonsterAttack()
{
	//애님 몽타주 재생(공격)
	if (AttackMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			AnimInstance->Montage_Play(AttackMontage);
			UE_LOG(LogTemp, Warning, TEXT("[DM] Play AttackMontage"));
		}
	}

	FVector Start = this->GetActorLocation();
	FVector End = Start + this->GetActorForwardVector() * Range;    //캐릭터 범위 전방
	//충돌 체크
	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepMultiByChannel(
		Hits,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,   //콜리전 채널 사용
		FCollisionShape::MakeSphere(50.f),  //근접 공격 반지름
		Params
	);
	//충돌 유효 체크
	if (bHit)
	{
		for (const FHitResult& H : Hits)
		{
			if (AActor* HitActor = H.GetActor())
			{   //자기 자신 제외 대미지 적용
				if (HitActor != this)
				{
					UGameplayStatics::ApplyDamage(HitActor, Damage, this->GetController(), this, UDamageType::StaticClass());
					UE_LOG(LogTemp, Warning, TEXT("ApplyDamage"));
				}
			}
		}
	}
}

void ATenEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATenEnemyCharacter::SetCurrentHealth(float NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("[DM] MonsterHealth is %f"), CurrentHealth);
	if (CurrentHealth <= 0.f)
	{
		// 몬스터 사망
		OnDeath();
	}
}

void ATenEnemyCharacter::OnDeath()
{
	if (bIsDead) return;
	bIsDead = true;

	// AIController 중지
	if (ATenAIController* AIController = Cast<ATenAIController>(GetController()))
	{
		AIController->StopAI(); // 수정 필요. 제대로 작동 안함.
	}

	// 애님 몽타주 재생(사망)
	if (DieMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && !AnimInstance->Montage_IsPlaying(DieMontage))
		{
			float AnimLength = AnimInstance->Montage_Play(DieMontage);

			FTimerHandle AnimEnd;
			GetWorld()->GetTimerManager().SetTimer(
				AnimEnd,
				this,
				&ATenEnemyCharacter::OnDeathMontageEnded,
				AnimLength,
				false
			);

			UE_LOG(LogTemp, Warning, TEXT("[DM] Play DieMontage"));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[DM] Monster is dead"));
}

void ATenEnemyCharacter::OnDeathMontageEnded()
{
	UE_LOG(LogTemp, Warning, TEXT("[DM] Destroy"));
	Destroy();
}
