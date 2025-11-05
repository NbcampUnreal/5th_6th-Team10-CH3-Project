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
    MeleeDamage = 40.f; //근접 대미지
    MeleeRange = 250.f;  //근접 거리

    MaxHealth = 200.f;                   //HP
    CurrentHealth = MaxHealth;  //현재 HP

    bCanAttack = true;           // 공격 가능 여부
    AttackCooldown = 1.5f;       // 후딜레이 시간
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();
    //오너 캐릭이 유효한지 확인과 무기 충돌 비활성화 
    if (OwningCharacter.IsValid())
    {   //무기 충돌 판정 제거 
        if (UStaticMeshComponent* Ranged = OwningCharacter->GetRangedWeaponMesh())
        {
            Ranged->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            Ranged->SetCollisionResponseToAllChannels(ECR_Ignore);  // 모든 채널 무시
            Ranged->SetSimulatePhysics(false);                      // 물리 반응 꺼짐
        }
        if (UStaticMeshComponent* Melee = OwningCharacter->GetMeleeWeaponMesh())
        {
            Melee->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            Melee->SetCollisionResponseToAllChannels(ECR_Ignore);
            Melee->SetSimulatePhysics(false);
        }
    }
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
    
    //가시성으로 든 무기를 시작을 근접무기로 확정 코드 추가
    CurrentWeaponType = EWeaponType::Melee;

    UE_LOG(LogTemp, Warning, TEXT("CombatComponent initialized for %s"), *CharacterOwner->GetName());
}

// ===== 플레이어에게 정의된 무기 접근 게터 =====
UStaticMeshComponent* UCombatComponent::GetRangedWeaponMesh() const { return OwningCharacter.IsValid() ? OwningCharacter->GetRangedWeaponMesh() : nullptr; }
UStaticMeshComponent* UCombatComponent::GetMeleeWeaponMesh() const { return OwningCharacter.IsValid() ? OwningCharacter->GetMeleeWeaponMesh() : nullptr; }
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
{   //후딜 중이면 리턴
    if (!OwningCharacter.IsValid() || !bCanAttack) return;
    bCanAttack = false;
    UE_LOG(LogTemp, Warning, TEXT("[DEBUG] StartFire() Called. WeaponType = %d"), (int32)CurrentWeaponType);

    //원거리 무기 장착시에만 사격
    if (CurrentWeaponType == EWeaponType::Ranged)
    {
        if (!CanFire()) return;
        //원거리 공격 몽타주 재생
        if (UAnimMontage* Montage = OwningCharacter->GetRangedAttackMontage())
            OwningCharacter->PlayAnimMontage(Montage);

        FireProjectile();
    }

    //Fire에 원거리 무기가 아니라면 근접 무기로 공격 함수 추가로 FIre하나로 원 근거리 공격 다 호출가능
    else if (CurrentWeaponType == EWeaponType::Melee)
    {
       if(UAnimMontage* Montage = OwningCharacter->GetMeleeAttackMontage())
           OwningCharacter->PlayAnimMontage(Montage);

        MeleeAttack();  //근접 공격 호출
    }

    // 공통 후딜
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle_ResetAttack,
        this,
        &UCombatComponent::ResetAttack,
        AttackCooldown,
        false
    ); 
}

//===== 발사체 =====
void UCombatComponent::FireProjectile()
{
    //총구 위치 총알 발사
    USceneComponent* Muzzle = GetMuzzleLocation();
    if (!Muzzle) return;
    
    UWorld* World = GetWorld();//GetWorld먼저 호출
    if (!World || !ProjectileClass) return; //불값 오류 방지
        
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
    if (!OwningCharacter.IsValid()) return;

    UE_LOG(LogTemp, Warning, TEXT("[DEBUG] MeleeAttack() called"));

    // 근접 몽타주 재생
    if (UAnimMontage* Montage = OwningCharacter->GetMeleeAttackMontage())
        OwningCharacter->PlayAnimMontage(Montage);

    // 충돌 검사
    FVector Start = OwningCharacter->GetActorLocation();
    FVector End = Start + OwningCharacter->GetActorForwardVector() * MeleeRange;

    TArray<FHitResult> Hits;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwningCharacter.Get());

    bool bHit = GetWorld()->SweepMultiByChannel(
        Hits,
        Start,
        End,
        FQuat::Identity,
        ECC_Pawn,  //  채널 사용
        FCollisionShape::MakeSphere(100.f),
        Params
    );

    DrawDebugSphere(GetWorld(), End, 80.f, 12, bHit ? FColor::Green : FColor::Red, false, 1.0f);
    //디버그용 충돌 영역
    UE_LOG(LogTemp, Warning, TEXT("MeleeAttack: bHit=%s, HitCount=%d"), bHit ? TEXT("true") : TEXT("false"), Hits.Num());

    if (bHit)
    {
        for (const FHitResult& H : Hits)
        {
            if (AActor* HitActor = H.GetActor())
            {
                if (HitActor != OwningCharacter.Get())
                {
                    UGameplayStatics::ApplyDamage(
                        HitActor,
                        MeleeDamage,
                        OwningCharacter->GetController(),
                        OwningCharacter.Get(),
                        UDamageType::StaticClass()
                    );
                    UE_LOG(LogTemp, Warning, TEXT("Hit: %s, Damage: %f"), *HitActor->GetName(), MeleeDamage);
                }
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

// ===== 공격 후딜 리셋 =====
void UCombatComponent::ResetAttack()
{
    bCanAttack = true;
    UE_LOG(LogTemp, Log, TEXT("Attack cooldown ended."));
}
