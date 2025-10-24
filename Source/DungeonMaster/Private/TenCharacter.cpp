#include "TenCharacter.h"
#include "Camera/CameraComponent.h"                                         //ī�޶� ����
#include "GameFramework/SpringArmComponent.h"                   //������ �� ����
#include "EnhancedInputComponent.h"                                            //UEnhancedInputComponent���
#include "TenPlayerController.h"                                                        //�÷��̾� ��Ʈ�ѷ� ȣ��
#include "GameFramework/CharacterMovementComponent.h"  //�÷��̾� �����Ʈ �Լ� ��� ��
#include "CombatComponent.h"                                                        //�Ĺ� ������Ʈ ���

ATenCharacter::ATenCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // ȸ�� ���� ,��Ʈ�ѷ��� ����
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // ===== ���� ������Ʈ ���� =====
    CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));

    // ===== ī�޶�, ������ �� =====
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 300.f;
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;

    // ===== ���� �޽� =====
    RangedWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RangedWeapon"));
    RangedWeaponMesh->SetupAttachment(GetMesh(), TEXT("RangedWeaponSocket"));

    MeleeWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeleeWeapon"));
    MeleeWeaponMesh->SetupAttachment(GetMesh(), TEXT("MeleeWeaponSocket"));

    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(RangedWeaponMesh);

    // ===== ĳ���� ���� =====
    MaxHealth = 100.f;
    Health = MaxHealth;
    NormalSpeed = 600.f;
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    // �̵� ���� ȸ��
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
}

void ATenCharacter::BeginPlay()
{
    Super::BeginPlay();

    // ���� ������Ʈ �ʱ�ȭ
    if (CombatComp)
        CombatComp->InitializeCombat(this);
}

//===== ĳ���� ���� ���� =====
void ATenCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Enhanced InputComponent�� ĳ����
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {   // IA�� �������� ���� ���� ���� ������Ʈ�ѷ��� ����� �� ��Ʈ�ѷ��� ĳ����
        if (ATenPlayerController* PC = Cast<ATenPlayerController>(GetController()))
        {
            // �̵�
            if (PC->MoveAction)
                EnhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Triggered, this, &ATenCharacter::Move);

            // ����
            if (PC->JumpAction)
            {
                EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Triggered, this, &ATenCharacter::StartJump);
                EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Completed, this, &ATenCharacter::StopJump);
            }

            // ī�޶� ȸ��
            if (PC->LookAction)
                EnhancedInput->BindAction(PC->LookAction, ETriggerEvent::Triggered, this, &ATenCharacter::Look);

            // ���� / ���� ����
            if (FireAction)
                EnhancedInput->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATenCharacter::StartFire);

            if (EquipRangedAction)
                EnhancedInput->BindAction(EquipRangedAction, ETriggerEvent::Triggered, this, &ATenCharacter::EquipRangedWeapon);

            if (EquipMeleeAction)
                EnhancedInput->BindAction(EquipMeleeAction, ETriggerEvent::Triggered, this, &ATenCharacter::EquipMeleeWeapon);
        }
    }
}

// ===== �̵� / ���� / ī�޶� ���� =====
void ATenCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    //��Ʈ�ѷ��� �ִ��� üũ
    if (!Controller) return;
    // ���� ��Ʈ�ѷ��� ȸ���� ������
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
    // ���� ���� ���
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    // ���� ���� ���
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    // ����/�Ĺ� �̵�
    AddMovementInput(ForwardDirection, MovementVector.X);
    // ��/���� �̵�
    AddMovementInput(RightDirection, MovementVector.Y);
}

//===== ���� ���� =====
void ATenCharacter::StartJump(const FInputActionValue& Value) 
{
    if (Value.Get<bool>()) Jump(); 
}
//===== ���� �� ======
void ATenCharacter::StopJump(const FInputActionValue& Value)
{
    if (Value.Get<bool>()) StopJumping(); 
}
//===== ���콺 ȸ�� =====
void ATenCharacter::Look(const FInputActionValue& Value)
{
    FVector2D v = Value.Get<FVector2D>();
    AddControllerYawInput(v.X);
    AddControllerPitchInput(v.Y);
}

// ===== ���� �Է� =====
//���Ÿ� �߻�
void ATenCharacter::StartFire(const FInputActionValue& Value)
{
    if (CombatComp) CombatComp->StartFire();
}
//���Ÿ� ���� ����
void ATenCharacter::EquipRangedWeapon(const FInputActionValue& Value)
{
    if (CombatComp) CombatComp->EquipRangedWeapon();
}
//���� ���� ����
void ATenCharacter::EquipMeleeWeapon(const FInputActionValue& Value)
{
    if (CombatComp) CombatComp->EquipMeleeWeapon();
}
