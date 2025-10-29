// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TenEnemyCharacter.generated.h"

class UCombatComponent;

UCLASS()
class DUNGEONMASTER_API ATenEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATenEnemyCharacter();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void MonsterAttack();

protected:
	virtual void BeginPlay() override;

	void SetCurrentHealth(float NewHealth);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float Range;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* HitReactMontage;

	void OnDeath();
	void OnDeathMontageEnded();

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DieMontage;
	bool bIsDead;
};
