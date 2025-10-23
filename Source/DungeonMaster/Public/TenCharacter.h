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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    USkeletalMeshComponent* RangedWeaponMesh;
    //근접무기
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    USkeletalMeshComponent* MeleeWeaponMesh;

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
    void StartFire(const FInputActionValue& Value);

    UFUNCTION()
    void EquipRangedWeapon(const FInputActionValue& Value);

    UFUNCTION()
    void EquipMeleeWeapon(const FInputActionValue& Value);

public:
    //  무기 관련 게터
    USkeletalMeshComponent* GetMeleeWeaponMesh() const { return MeleeWeaponMesh; }
    USkeletalMeshComponent* GetRangedWeaponMesh() const { return RangedWeaponMesh; }
    USceneComponent* GetMuzzleLocation() const { return MuzzleLocation; }
};
