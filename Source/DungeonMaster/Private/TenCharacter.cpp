#include "TenCharacter.h"
#include "Camera/CameraComponent.h"                                         //카메라 구현
#include "GameFramework/SpringArmComponent.h"                   //스프링 암 구현
#include "EnhancedInputComponent.h"                                            //UEnhancedInputComponent사용
#include "TenPlayerController.h"                                                        //플레이어 컨트롤러 호출
#include "GameFramework/CharacterMovementComponent.h"  //플레이어 무브먼트 함수 사용 시
#include "CombatComponent.h"                                                        //컴뱃 컴포넌트 사용
#include "TimerManager.h"

ATenCharacter::ATenCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // 회전 설정 ,컨트롤러와 독립
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // ===== 전투 컴포넌트 생성 =====
    CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));

    // ===== 카메라, 스프링 암 =====
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 300.f;
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;

    // ===== 무기 메시 =====
    RangedWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RangedWeapon"));
    RangedWeaponMesh->SetupAttachment(GetMesh(), TEXT("RangedWeaponSocket"));

    MeleeWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeleeWeapon"));
    MeleeWeaponMesh->SetupAttachment(GetMesh(), TEXT("MeleeWeaponSocket"));

    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(RangedWeaponMesh, TEXT("Muzzle"));

    // ===== 캐릭터 스탯 =====
    MaxHealth = 100.f;
    Health = MaxHealth;
    NormalSpeed = 700.f;
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    // 이동 방향 회전
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 800.f, 0.f);

    //=====회피 관련=====
   
    //회피 이동거리
    DodgeDistance = 600.f;
    //회피 지속 시간
    DodgeDuration = 0.2f;

}

void ATenCharacter::BeginPlay()
{
    Super::BeginPlay();
    // 전투 컴포넌트 초기화
    if (CombatComp)
        CombatComp->InitializeCombat(this);

    //게임시작시 무기 숨김 설정
    if (UStaticMeshComponent* Ranged = GetRangedWeaponMesh())
    {
        Ranged->SetHiddenInGame(true);   // 원거리 무기 숨김
    }

    if (UStaticMeshComponent* Melee = GetMeleeWeaponMesh())
    {
        Melee->SetHiddenInGame(false);   // 근접 무기 보이게
    }
}

//===== 캐릭터 조작 구현 =====
void ATenCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {   // IA를 가져오기 위해 현재 소유 중인컨트롤러를 만들어 두 컨트롤러로 캐스팅
        if (ATenPlayerController* PC = Cast<ATenPlayerController>(GetController()))
        {  
            // 이동
            if (PC->MoveAction)
                EnhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Triggered, this, &ATenCharacter::Move);

            // 점프
            if (PC->JumpAction)
            {
                EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Triggered, this, &ATenCharacter::StartJump);
                EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Completed, this, &ATenCharacter::StopJump);
            }

            // 카메라 회전
            if (PC->LookAction)
                EnhancedInput->BindAction(PC->LookAction, ETriggerEvent::Triggered, this, &ATenCharacter::Look);

            // 공격 / 무기 장착
            if (FireAction)
                EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &ATenCharacter::StartFire);

            //무기 불러오기
            if (EquipRangedAction)
                EnhancedInput->BindAction(EquipRangedAction, ETriggerEvent::Triggered, this, &ATenCharacter::EquipRangedWeapon);

            if (EquipMeleeAction)
                EnhancedInput->BindAction(EquipMeleeAction, ETriggerEvent::Triggered, this, &ATenCharacter::EquipMeleeWeapon);

            //회피 
            if(PC->DodgeAction)
                EnhancedInput->BindAction(PC->DodgeAction, ETriggerEvent::Started, this, &ATenCharacter::Dodge);
        }
    }
}

// ===== 이동 / 점프 / 카메라 시점 =====
void ATenCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    //컨트롤러가 있는지 체크
    if (!Controller) return;
    //회피 방향용 이동 입력 저장
    LastMoveInput = MovementVector;
    // 현재 컨트롤러의 회전을 가져옴
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
    // 전방 벡터 계산
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    // 우측 벡터 계산
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    // 전방/후방 이동
    AddMovementInput(ForwardDirection, MovementVector.X);
    // 좌/우측 이동
    AddMovementInput(RightDirection, MovementVector.Y);
}

//===== 점프 시작 =====
void ATenCharacter::StartJump(const FInputActionValue& Value) 
{
    if (Value.Get<bool>()) Jump(); 
}
//===== 점프 끝 ======
void ATenCharacter::StopJump(const FInputActionValue& Value)
{
    if (Value.Get<bool>()) StopJumping(); 
}
//===== 마우스 회전 =====
void ATenCharacter::Look(const FInputActionValue& Value)
{
    FVector2D v = Value.Get<FVector2D>();
    AddControllerYawInput(v.X);
    AddControllerPitchInput(v.Y);
}

// ===== 전투 입력 =====
//원거리 발사
void ATenCharacter::StartFire()
{
    if (CombatComp) CombatComp->StartFire();
}
//원거리 무기 착용
void ATenCharacter::EquipRangedWeapon(const FInputActionValue& Value)
{
    if (CombatComp) CombatComp->EquipRangedWeapon();
}
//근접 무기 착용
void ATenCharacter::EquipMeleeWeapon(const FInputActionValue& Value)
{
    if (CombatComp) CombatComp->EquipMeleeWeapon();
}


//===== 몽타주 게터 정의=====
UAnimMontage* ATenCharacter::GetRangedAttackMontage() const
{
    return RangedAttackMontage;
}

UAnimMontage* ATenCharacter::GetMeleeAttackMontage() const
{
    return MeleeAttackMontage;
}

//===== 대미지 =====
float ATenCharacter::TakeDamage(float DamageAmount,
    FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    //회피 중이면 데미지 무시
    if (CombatComp && CombatComp->IsInvincible())
    {
        UE_LOG(LogTemp, Warning, TEXT("Damage ignored (Invincible during Dodge)"));
        return 0.f;
    }

    Health -= DamageAmount;
    Health = FMath::Max(0.f, Health);

    UE_LOG(LogTemp, Warning, TEXT("%s took %f damage, health now %f"), *GetName(), DamageAmount, Health);

    if (Health <= 0.f)
    {
        OnDeath();
    }
    return ActualDamage;
}

//=====사망처리=====
void ATenCharacter::OnDeath()
{//사망처리 몽타주 
    if (DeathMontage)
    {
        PlayAnimMontage(DeathMontage);
        //몽타주 길이만큼 대기하고 Destroy
        float Delay = DeathMontage->GetPlayLength();
        GetWorldTimerManager().SetTimerForNextTick([this, Delay]()
            {
                FTimerHandle Handle;
                GetWorldTimerManager().SetTimer(Handle, [this]() {
                    Destroy();
                    }, Delay, false);
            });
    }
    else
    {
        Destroy();
    }
}

//=====회피 로직=====
void ATenCharacter::Dodge()
{
    //회피 중복 방지
    if (bIsDodging || !Controller || !CombatComp) return;

    bIsDodging = true;
    CombatComp->SetInvincible(true);

    //  현재 입력된 방향(WASD) 기준으로 회피 
    const FRotator ControlRot = Controller->GetControlRotation();
    const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

    // 입력 벡터 기준으로 실제 방향 처리
    FVector DodgeDir = FVector::ZeroVector;
    if (!LastMoveInput.IsNearlyZero())
    {
        FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
        FVector Right = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
        DodgeDir = (Forward * LastMoveInput.X + Right * LastMoveInput.Y).GetSafeNormal();
    }
    else
    {
        // 입력 없으면 전방 회피
        DodgeDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    }

    //회피 이동 속도
    const float Speed = DodgeDistance / FMath::Max(0.01f, DodgeDuration);
    const FVector DashVelocity = DodgeDir * Speed;

    //회피 마찰 제거
    if (auto* Move = GetCharacterMovement())
    {
        SavedBrakingFrictionFactor = Move->BrakingFrictionFactor;
        Move->BrakingFriction = 0.f;
        Move->GroundFriction = 0.f;
        Move->MaxAcceleration = 999999.f;
    }
    //캐릭터 앞으로 이동
    LaunchCharacter(DashVelocity, true, true);

    //일정 시간 후 회피 종료
    GetWorldTimerManager().SetTimer(Timer_DodgeEnd, [this]()
        {
            bIsDodging = false;

            if (CombatComp)
                CombatComp->SetInvincible(false); // 무적 해제

            if (auto* Move = GetCharacterMovement())
            {
                Move->StopMovementImmediately();
                Move->BrakingFrictionFactor = SavedBrakingFrictionFactor;
                Move->GroundFriction = 8.f;
                Move->MaxAcceleration = 2048.f;
            }

        }, DodgeDuration, false);
}
