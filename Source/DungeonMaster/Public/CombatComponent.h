#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

//무기 타입 열거형(UI, 게임매니저 사용 가능)
UENUM(BlueprintType)
enum class EWeaponType : uint8
{   // UMETA 언리얼 에디터의 드롭다운 메뉴에 표시
    None   UMETA(DisplayName = "None"),         //맨손
    Ranged UMETA(DisplayName = "Ranged"),   //원거리무기
    Melee  UMETA(DisplayName = "Melee")        //근첩무기
};

// =============전방 선언==============
class ATenCharacter;                        //캐릭터 전방 선언 (CombatComponent가 플레이어 캐릭터 참조용)
class UStaticMeshComponent;         // 무기들이 전부 스태틱 매시
class USceneComponent;                 // 루트컴포넌트
class UAnimMontage;                     // 애니메이션 몽타주 사용
//class AProjectile;                              //발사체

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DUNGEONMASTER_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCombatComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // 해당 컴포넌트가 부착된 캐릭터 액터를 참조 설정
    UFUNCTION(BlueprintCallable)
    void InitializeCombat(ATenCharacter* CharacterOwner);

    // 무기 스왑
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipRangedWeapon();
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipMeleeWeapon();

    // 공격
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StartFire();       // 총 공격
    void StopFire();        // 총 공격 종료 
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void MeleeAttack(); // 근접 공격

    // 체력/데미지
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void ApplyDamageToCharacter(float DamageAmount);

    // ===== 상태 =====
    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bIsMeleeAttacking = false; // 애니메이션 블루프린트 연동용

protected:
    //플레이어가 조종하고 있는 캐릭터
    TWeakObjectPtr<ATenCharacter> OwningCharacter;

    //현재 장착된 무기 타입
    UPROPERTY(BlueprintReadWrite)
    EWeaponType CurrentWeaponType;
    float FireRate;             //발사속도
    float LastFireTime;     //발사제한
    float MeleeDamage;  //근접 데미지
    float MeleeRange;      //근접 거리

    bool bCanAttack;                     // 공격 가능 여부
    float AttackCooldown;                // 후딜 시간
    FTimerHandle TimerHandle_ResetAttack; // 타이머 핸들

    void ResetAttack();                  // 후딜 끝내는 함수

    //총알 발사체 ,총알 자체에 대미지 줄 예정
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    TSubclassOf<AActor> ProjectileClass;
    //근접공격 몽타주 몽타주 캐릭터로 이전
    //UPROPERTY(BlueprintReadWrite, Category = "Combat")
    //UAnimMontage* MeleeAttackMontage;
    //UPROPERTY(BlueprintReadWrite, Category = "Combat")
    //UAnimMontage* RangedAttackMontage;
    
    float MaxHealth;        //최대HP
    float CurrentHealth;  //현재HP

    //캐릭터에서 쓰는 게터 함수들
    UStaticMeshComponent* GetRangedWeaponMesh() const;
    UStaticMeshComponent* GetMeleeWeaponMesh() const;
    USceneComponent* GetMuzzleLocation() const;
    bool CanFire() const;
    //총알 함수
    void FireProjectile();  
    //무기 스왑 함수
    void SwapWeapon(EWeaponType NewWeaponType);
};
