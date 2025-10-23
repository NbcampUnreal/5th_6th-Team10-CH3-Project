#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TenCharacter.generated.h"

// ���� ����
class USpringArmComponent;
class UCameraComponent;
class UCombatComponent;
struct FInputActionValue;
class UInputAction;

// ĳ���� Ŭ����
UCLASS()
class DUNGEONMASTER_API ATenCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ATenCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // =====ī�޶�=====
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* CameraComp;

    // =====���� ������Ʈ ��,���� ����=====
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    UCombatComponent* CombatComp;

    //===== ���� �޽� ���Ͽ� ������=====
    //���Ÿ� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    USkeletalMeshComponent* RangedWeaponMesh;
    //��������
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    USkeletalMeshComponent* MeleeWeaponMesh;

    // �Ѿ� �߻� ��ġ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    USceneComponent* MuzzleLocation;

    // ===== ĳ���� ���� =====
    //�ִ� HP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth;
    // HP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Health;
    //�̵� �ӵ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float NormalSpeed;

    // ===== ���� �Է� =====
    //���Ÿ� ���� �߻�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* FireAction;
    // ���Ÿ� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* EquipRangedAction;
    //���� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* EquipMeleeAction;

    // ===== �̵��� ���� =====
    UFUNCTION()
    void Move(const FInputActionValue& Value);

    UFUNCTION()
    void StartJump(const FInputActionValue& Value);

    UFUNCTION()
    void StopJump(const FInputActionValue& Value);

    UFUNCTION()
    void Look(const FInputActionValue& Value);

    //===== ���� �߻� ,�m�� �Լ�=====
    UFUNCTION()
    void StartFire(const FInputActionValue& Value);

    UFUNCTION()
    void EquipRangedWeapon(const FInputActionValue& Value);

    UFUNCTION()
    void EquipMeleeWeapon(const FInputActionValue& Value);

public:
    //  ���� ���� ����
    USkeletalMeshComponent* GetMeleeWeaponMesh() const { return MeleeWeaponMesh; }
    USkeletalMeshComponent* GetRangedWeaponMesh() const { return RangedWeaponMesh; }
    USceneComponent* GetMuzzleLocation() const { return MuzzleLocation; }
};
