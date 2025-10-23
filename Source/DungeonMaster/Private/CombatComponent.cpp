#include "CombatComponent.h"
#include "TenCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    //���� �⺻��
    CurrentWeaponType = EWeaponType::None;
    FireRate = 1.0f;            //�ʴ� �߻� Ƚ��
    LastFireTime = 0.0f;    //�߻� ����
    MeleeDamage = 30.f; //���� �����
    MeleeRange = 150.f;  //���� �Ÿ�

    MaxHealth = 100.f;                   //HP
    CurrentHealth = MaxHealth;  //���� HP
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();
    //���� ĳ���� ��ȿ���� Ȯ��
    if (OwningCharacter.IsValid())
        SwapWeapon(EWeaponType::Melee); // �⺻ ����: ����
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
   
//=====�÷��̾� ���� �ʱ�ȭ=====
void UCombatComponent::InitializeCombat(ATenCharacter* CharacterOwner)
{
    if (!CharacterOwner) return;
    OwningCharacter = CharacterOwner;
    UE_LOG(LogTemp, Warning, TEXT("CombatComponent initialized for %s"), *CharacterOwner->GetName());
}

// ===== �÷��̾�� ���ǵ� ���� ���� =====
USkeletalMeshComponent* UCombatComponent::GetRangedWeaponMesh() const { return OwningCharacter.IsValid() ? OwningCharacter->GetRangedWeaponMesh() : nullptr; }
USkeletalMeshComponent* UCombatComponent::GetMeleeWeaponMesh() const { return OwningCharacter.IsValid() ? OwningCharacter->GetMeleeWeaponMesh() : nullptr; }
USkeletalMeshComponent* UCombatComponent::GetCharacterMesh() const { return OwningCharacter.IsValid() ? OwningCharacter->GetMesh() : nullptr; }
USceneComponent* UCombatComponent::GetMuzzleLocation() const { return OwningCharacter.IsValid() ? OwningCharacter->GetMuzzleLocation() : nullptr; }

// ===== ���� ���� =====
void UCombatComponent::EquipRangedWeapon() { SwapWeapon(EWeaponType::Ranged); }
void UCombatComponent::EquipMeleeWeapon() { SwapWeapon(EWeaponType::Melee); }

void UCombatComponent::SwapWeapon(EWeaponType NewWeaponType)
{
    if (!OwningCharacter.IsValid()) return;
    if (CurrentWeaponType == NewWeaponType) return;

    if (USkeletalMeshComponent* Ranged = GetRangedWeaponMesh()) Ranged->SetHiddenInGame(true);
    if (USkeletalMeshComponent* Melee = GetMeleeWeaponMesh())  Melee->SetHiddenInGame(true);

    if (NewWeaponType == EWeaponType::Ranged)
        if (USkeletalMeshComponent* Ranged = GetRangedWeaponMesh()) Ranged->SetHiddenInGame(false);
        else if (NewWeaponType == EWeaponType::Melee)
            if (USkeletalMeshComponent* Melee = GetMeleeWeaponMesh()) Melee->SetHiddenInGame(false);

    CurrentWeaponType = NewWeaponType;
}

// ===== �߻� �Լ� =====
bool UCombatComponent::CanFire() const
{
    UWorld* W = GetWorld();
    return W && (UGameplayStatics::GetTimeSeconds(W) - LastFireTime) >= (1.f / FireRate);
}

// ===== ��� �Լ� =====
void UCombatComponent::StartFire()
{
    if (!OwningCharacter.IsValid()) return;
    //���Ÿ� ���� �����ÿ��� ���
    if (CurrentWeaponType != EWeaponType::Ranged || !ProjectileClass) return;
    if (!CanFire()) return;
    //�ѱ� ��ġ �Ѿ� �߻�
    USceneComponent* Muzzle = GetMuzzleLocation();
    if (!Muzzle) return;

    UWorld* World = GetWorld();
    if (!World) return;

    FVector SpawnLocation = Muzzle->GetComponentLocation();
    FRotator SpawnRotation = OwningCharacter->GetControlRotation();

    World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation);
    LastFireTime = UGameplayStatics::GetTimeSeconds(World);
}
//===== ��� ���� =====
void UCombatComponent::StopFire() {}

//===== ���� ���� =====
void UCombatComponent::MeleeAttack()
{
    if (!OwningCharacter.IsValid() || CurrentWeaponType != EWeaponType::Melee) return;

    bIsMeleeAttacking = true; // �ִϸ��̼� �������Ʈ���� ���

    // ��Ÿ�� ���
    if (MeleeAttackMontage)
        OwningCharacter->PlayAnimMontage(MeleeAttackMontage);
    //���� ����
    FVector Start = OwningCharacter->GetActorLocation();
    FVector End = Start + OwningCharacter->GetActorForwardVector() * MeleeRange;    //ĳ���� ���� ����
    //�浹 üũ
    TArray<FHitResult> Hits;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwningCharacter.Get());

    bool bHit = GetWorld()->SweepMultiByChannel(
        Hits,
        Start,
        End,
        FQuat::Identity,
        ECC_Pawn,   //�ݸ��� ä�� ���
        FCollisionShape::MakeSphere(50.f),  //���� ���� ������
        Params
    );
    //�浹 ��ȿ üũ
    if (bHit)
    {
        for (const FHitResult& H : Hits)
        {
            if (AActor* HitActor = H.GetActor())
            {   //�ڱ� �ڽ� ���� ����� ����
                if (HitActor != OwningCharacter.Get())
                    UGameplayStatics::ApplyDamage(HitActor, MeleeDamage, OwningCharacter->GetController(), OwningCharacter.Get(), UDamageType::StaticClass());
            }
        }
    }
}
  // ===== ü�� ���� �Լ� =====
void UCombatComponent::ApplyDamageToCharacter(float DamageAmount)
{
    CurrentHealth -= DamageAmount;
    CurrentHealth = FMath::Max(0.f, CurrentHealth);

    if (CurrentHealth <= 0.f && OwningCharacter.IsValid())
    {
        // ĳ���� ��� ó��
        OwningCharacter->Destroy();
    }
}
