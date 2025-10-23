#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

//���� Ÿ�� ������(UI, ���ӸŴ��� ��� ����)
UENUM(BlueprintType)
enum class EWeaponType : uint8
{   // UMETA �𸮾� �������� ��Ӵٿ� �޴��� ǥ��
    None   UMETA(DisplayName = "None"),         //�Ǽ�
    Ranged UMETA(DisplayName = "Ranged"),   //���Ÿ�����
    Melee  UMETA(DisplayName = "Melee")        //��ø����
};

// =============���� ����==============
class ATenCharacter;                        //ĳ���� ���� ���� (CombatComponent�� �÷��̾� ĳ���� ������)
class USkeletalMeshComponent;   // �ִϸ��̼� ��Ÿ�� ���
class USceneComponent;                 // ���̷�Ż �޽� ������Ʈ ���
class UAnimMontage;                     // ��Ʈ ������Ʈ ���

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DUNGEONMASTER_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCombatComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // �ش� ������Ʈ�� ������ ĳ���� ���͸� ���� ����
    void InitializeCombat(ATenCharacter* CharacterOwner);

    // ���� ����
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipRangedWeapon();
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipMeleeWeapon();

    // ����
    void StartFire();       // �� ����
    void StopFire();        // �� ���� ���� (�ɼ�)
    void MeleeAttack(); // ���� ����

    // ü��/������
    UFUNCTION(BlueprintCallable, Category = "Stats")
    void ApplyDamageToCharacter(float DamageAmount);

    // ===== ���� =====
    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bIsMeleeAttacking = false; // �ִϸ��̼� �������Ʈ ������

protected:
    //�÷��̾ �����ϰ� �ִ� ĳ����
    TWeakObjectPtr<ATenCharacter> OwningCharacter;

    //���� ������ ���� Ÿ��
    EWeaponType CurrentWeaponType;
    float FireRate;             //�߻�ӵ�
    float LastFireTime;     //�߻�����
    float MeleeDamage;  //���� ������
    float MeleeRange;      //���� �Ÿ�

    //�Ѿ� �߻�ü ,�Ѿ� ��ü�� ����� �� ����
    UPROPERTY(EditDefaultsOnly, Category = "Combat")
    TSubclassOf<AActor> ProjectileClass;
    //�������� ��Ÿ��
    UPROPERTY(BlueprintReadWrite, Category = "Combat")
    UAnimMontage* MeleeAttackMontage;
    
    float MaxHealth;        //�ִ�HP
    float CurrentHealth;  //����HP

    //ĳ���Ϳ��� ���� ���� �Լ���
    USkeletalMeshComponent* GetRangedWeaponMesh() const;
    USkeletalMeshComponent* GetMeleeWeaponMesh() const;
    USkeletalMeshComponent* GetCharacterMesh() const;
    USceneComponent* GetMuzzleLocation() const;
    bool CanFire() const;
    //���� ���� �Լ�
    void SwapWeapon(EWeaponType NewWeaponType);
};
