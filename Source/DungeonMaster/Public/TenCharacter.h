#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TenCharacter.generated.h"

// 전방 선언
class USpringArmComponent;
class UCameraComponent;
class UCombatComponent;
struct FInputActionValue;
class UInputAction;
class UStaticMeshComponent;
class UWidgetComponent;
class UUserWidget;

// 캐릭터 클래스
UCLASS()
class DUNGEONMASTER_API ATenCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATenCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
   
    //대미지
    virtual float TakeDamage(
        float DamageAmount, 
        struct FDamageEvent const& DamageEvent, 
        AController* EventInstigator, 
        AActor* DamageCauser) override;
    
    //사망 처리
    void OnDeath();

    //캐릭터 사망 몽타주
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* DeathMontage;

    // =====카메라=====
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComp;

    // =====전투 컴포넌트 총,근접 공격=====
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    UCombatComponent* CombatComp;

    //===== 무기 메시 소켓에 부착용=====
    //원거리 무기
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    UStaticMeshComponent* RangedWeaponMesh;
    //근접무기
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
    UStaticMeshComponent* MeleeWeaponMesh;

    // 총알 발사 위치
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    USceneComponent* MuzzleLocation;

    // ===== 캐릭터 스탯 =====
    //최대 HP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth;
    // HP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Health;
    //이동 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float NormalSpeed;

    // ===== 공격 입력 =====
    //원거리 무기 발사
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* FireAction;
    // 원거리 무기 착용
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* EquipRangedAction;
    //근접 무기 착용
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* EquipMeleeAction;

    //===== 애님 몽타주 보관 무기 메시 없음=====
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animetion")
    UAnimMontage* RangedAttackMontage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animetion")
    UAnimMontage* MeleeAttackMontage;

    // ===== 이동과 시점 =====
    UFUNCTION()
    void Move(const FInputActionValue& Value);

    UFUNCTION()
    void StartJump(const FInputActionValue& Value);

    UFUNCTION()
    void StopJump(const FInputActionValue& Value);

    UFUNCTION()
    void Look(const FInputActionValue& Value);

    //===== 무기 발사 ,챡용 함수=====
    UFUNCTION()
    void StartFire();

    UFUNCTION()
    void EquipRangedWeapon(const FInputActionValue& Value);

    UFUNCTION()
    void EquipMeleeWeapon(const FInputActionValue& Value);

    //=====회피 입력=====

    //회피 함수
    UFUNCTION()
    void Dodge();

    //회피 이동거리
    UPROPERTY(EditAnywhere, Category = "Dodge")
    float DodgeDistance ;

    //회피 지속시간
    UPROPERTY(EditAnywhere, Category = "Dodge")
    float DodgeDuration;

    //회피 중인지 여부
    bool bIsDodging = false;
    //회피 종료 타이머
    float SavedBrakingFrictionFactor = 0.f;
    FTimerHandle Timer_DodgeEnd;
    //이동 방향에 따른 회피
    FVector2D LastMoveInput = FVector2D::ZeroVector;
public:
    //  무기 관련 게터
    UStaticMeshComponent* GetMeleeWeaponMesh() const { return MeleeWeaponMesh; }
    UStaticMeshComponent* GetRangedWeaponMesh() const { return RangedWeaponMesh; }
    USceneComponent* GetMuzzleLocation() const { return MuzzleLocation; }
    //애님 몽타주 게터
    UAnimMontage* GetRangedAttackMontage() const;
    UAnimMontage* GetMeleeAttackMontage() const;

    //골드 획득
    UFUNCTION()
    void HandleEnemyKilled(int32 GoldAmount);

    //====Ui====
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* OverheadWidget;

    void UpdateOverheadHP();
    
    //골드 UI 인스턴스
    UPROPERTY()
    UUserWidget* TotalGoldWidGet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> TotalGoldWidgetClass;//WBP_Total클래스 참조

    void UpdateTotalGoldUI(); //골드 갱신
};
