#include "CombatComponent.h"
#include "TenCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    //무기 기본값
    CurrentWeaponType = EWeaponType::None;
    FireRate = 1.0f;            //초당 발사 횟수
    LastFireTime = 0.0f;    //발사 제한
    MeleeDamage = 30.f; //근접 대미지
    MeleeRange = 150.f;  //근접 거리

    MaxHealth = 100.f;                   //HP
    CurrentHealth = MaxHealth;  //현재 HP
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();
    //오너 캐릭이 유효한지 확인
    if (OwningCharacter.IsValid())
    {}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
   
//=====플레이어 전투 초기화=====
void UCombatComponent::InitializeCombat(ATenCharacter* CharacterOwner)
{
    if (!CharacterOwner) return;
    OwningCharacter = CharacterOwner;
    UE_LOG(LogTemp, Warning, TEXT("CombatComponent initialized for %s"), *CharacterOwner->GetName());
}

// ===== 플레이어에게 정의된 무기 접근 게터 =====
UStaticMeshComponent* UCombatComponent::GetRangedWeaponMesh() const { return OwningCharacter.IsValid() ? OwningCharacter->GetRangedWeaponMesh() : nullptr; }
UStaticMeshComponent* UCombatComponent::GetMeleeWeaponMesh() const { return OwningCharacter.IsValid() ? OwningCharacter->GetMeleeWeaponMesh() : nullptr; }
//USkeletalMeshComponent* UCombatComponent::GetCharacterMesh() const { return OwningCharacter.IsValid() ? OwningCharacter->GetMesh() : nullptr; }
USceneComponent* UCombatComponent::GetMuzzleLocation() const { return OwningCharacter.IsValid() ? OwningCharacter->GetMuzzleLocation() : nullptr; }

// ===== 무기 스왑 =====
void UCombatComponent::EquipRangedWeapon() { SwapWeapon(EWeaponType::Ranged); }
void UCombatComponent::EquipMeleeWeapon() { SwapWeapon(EWeaponType::Melee); }

void UCombatComponent::SwapWeapon(EWeaponType NewWeaponType)
{
    if (!OwningCharacter.IsValid() || CurrentWeaponType == NewWeaponType) return;

    // 모든 무기 숨기기
    if (UStaticMeshComponent* Ranged = GetRangedWeaponMesh())
        Ranged->SetHiddenInGame(true);

    if (UStaticMeshComponent* Melee = GetMeleeWeaponMesh())
        Melee->SetHiddenInGame(true);

    // 새 무기 타입만 보이게
    switch (NewWeaponType)
    {
    case EWeaponType::Ranged:
        if (UStaticMeshComponent* Ranged = GetRangedWeaponMesh())
            Ranged->SetHiddenInGame(false);
        break;

    case EWeaponType::Melee:
        if (UStaticMeshComponent* Melee = GetMeleeWeaponMesh())
            Melee->SetHiddenInGame(false);
        break;
    }

    CurrentWeaponType = NewWeaponType;
}
// ===== 발사 함수 =====
bool UCombatComponent::CanFire() const
{
    UWorld* W = GetWorld();
    return W && (UGameplayStatics::GetTimeSeconds(W) - LastFireTime) >= (1.f / FireRate);
}

// ===== 사격 함수 =====
void UCombatComponent::StartFire()
{
    if (!OwningCharacter.IsValid()) return;
    //원거리 무기 장착시에만 사격
    if (CurrentWeaponType == EWeaponType::Ranged)
    {   //투사체 검사
        if (!CanFire()) return;
        if (OwningCharacter.IsValid())
        {
            UAnimMontage* Montage = OwningCharacter->GetRangedAttackMontage();
            if (Montage)
            {
                UE_LOG(LogTemp, Log, TEXT("Playing RangedAttackMontage"));
                OwningCharacter->PlayAnimMontage(Montage);
            }
        }
        // 몽타주 캐릭터에서 가져와서 재생
        FireProjectile();
    }
    //Fire에 원거리 무기가 아니라면 근접 무기로 공격 함수 추가로 FIre하나로 원 근거리 공격 다 호출가능
    else if (CurrentWeaponType == EWeaponType::Melee)
    {
        MeleeAttack();  //근접 공격 호출
    }
}

//===== 발사체 =====
void UCombatComponent::FireProjectile()
{
    //총구 위치 총알 발사
    USceneComponent* Muzzle = GetMuzzleLocation();
    if (!Muzzle) return;
    
    UWorld* World = GetWorld();//GetWorld먼저 호출
    if (!World || ProjectileClass) return; //불값 오류 방지
        
    FVector SpawnLocation = Muzzle->GetComponentLocation();
    FRotator SpawnRotation = OwningCharacter.IsValid() ? OwningCharacter->GetControlRotation() : FRotator::ZeroRotator;
    World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation);
    LastFireTime = UGameplayStatics::GetTimeSeconds(World);
 }
 
//===== 사격 종료 =====
void UCombatComponent::StopFire() {}

//===== 근접 공격 =====
void UCombatComponent::MeleeAttack()
{
    if (!OwningCharacter.IsValid() || CurrentWeaponType != EWeaponType::Melee) return;

    bIsMeleeAttacking = true; // 애니메이션 블루프린트에서 사용

    if (!OwningCharacter.IsValid())
    {// 몽타주 재생
        UAnimMontage* Montage = OwningCharacter->GetMeleeAttackMontage();
        if (Montage)
        {
            UE_LOG(LogTemp, Log, TEXT("Playing MeleeAttackMontage"));
            OwningCharacter->PlayAnimMontage(Montage);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MeleeAttackMontage is null on character"));
        }
    }   
         //공격 시작
        FVector Start = OwningCharacter->GetActorLocation();
        FVector End = Start + OwningCharacter->GetActorForwardVector() * MeleeRange;    //캐릭터 범위 전방
        //충돌 체크
        TArray<FHitResult> Hits;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(OwningCharacter.Get());

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
                    if (HitActor != OwningCharacter.Get())
                        UGameplayStatics::ApplyDamage(HitActor, MeleeDamage, OwningCharacter->GetController(), OwningCharacter.Get(), UDamageType::StaticClass());
                }
            }
        }
}


  // ===== 체력 관련 함수 =====
void UCombatComponent::ApplyDamageToCharacter(float DamageAmount)
{
    CurrentHealth -= DamageAmount;
    CurrentHealth = FMath::Max(0.f, CurrentHealth);

    if (CurrentHealth <= 0.f && OwningCharacter.IsValid())
    {
        // 캐릭터 사망 처리
        OwningCharacter->Destroy();
    }
}
