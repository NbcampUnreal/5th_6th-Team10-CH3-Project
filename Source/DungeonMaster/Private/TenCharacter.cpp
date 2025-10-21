#include "TenCharacter.h"
#include "Camera/CameraComponent.h"											//스프링 암, 카메라 구현시
#include "GameFramework/SpringArmComponent.h"						//스프링 암, 카메라 구현시
#include "EnhancedInputComponent.h"												//UEnhancedInputComponent 사용 시									
#include "TenPlayerController.h"															//플레이어 컨트롤러 사용
#include "GameFramework/CharacterMovementComponent.h"		//플레이어 무브먼트 함수사용 시

ATenCharacter::ATenCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// 캐릭터가 컨트롤러 회전에 영향을 받지 않도록 설정 (카메라만 회전)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//=========스프링 암 ===========

	//스프링 암 생성후 루트에 부착
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	//캐릭터와 카메라 사이 거리
	SpringArmComp->TargetArmLength = 300.0f;
	//컨트롤러 회전에 따라 스프링 암도 회전 설정
	SpringArmComp->bUsePawnControlRotation = true;

	//=========카메라 ==========

	//카메라 생성 후 스프링 암에 부착
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	//스프링 암의 회전을 따름으로 false;
	CameraComp->bUsePawnControlRotation = false;

	//==========캐릭터 정보============

	//캐릭터 HP
	MaxHealth = 100.f;
	Health = MaxHealth;
    //이동 속도
    NormalSpeed = 600.0f;
    //캐릭터 기본 속도 설정
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    // ========캐릭터 이동 컴포넌트 설정=====
    // 캐릭터가 이동 방향으로 회전하도록 설정.
    GetCharacterMovement()->bOrientRotationToMovement = true;
    // 캐릭터 회전 속도를 설정합니다. (초당 540도)
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    // 캐릭터가 공중에서도 이동 방향으로 회전할지 여부 (필요에 따라 설정)
    GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false; //  false가 기본
}

// =========캐릭터 조작 구현 ===============
void ATenCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA를 가져오기 위해 현재 소유 중인컨트롤러를 만들어 두 컨트롤러로 캐스팅
        if (ATenPlayerController* PlayerController = Cast<ATenPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ATenCharacter::Move);
            }

            //점프
            if (PlayerController->JumpAction)
            {
                // IA_Jump 액션 키를 "키를 누르고 있는 동안" StartJump() 호출
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ATenCharacter::StartJump);
                // IA_Jump 액션 키에서 "손을 뗀 순간" StopJump() 호출
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ATenCharacter::StopJump);
            }
            //룩
            if (PlayerController->LookAction)
            {
                // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ATenCharacter::Look);
            }
        }
    }
}

//==============이동 함수===============
void ATenCharacter::Move(const FInputActionValue& value)
{
    FVector2D MovementVector = value.Get<FVector2D>();

    float ForwardValue = MovementVector.X; // X축을 전방으로
    float RightValue = MovementVector.Y;   // Y축을 오른쪽으로
    //컨트롤러가 있는지 체크
    if (Controller != nullptr)
    {
        // 현재 컨트롤러의 회전을 가져옵니다.
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0); // Yaw 값만 사용

        // 전방 벡터 계산
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        // 우측 벡터 계산
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // 전방/후방 이동
        AddMovementInput(ForwardDirection, ForwardValue);
        // 좌/우측 이동
        AddMovementInput(RightDirection, RightValue);
    }
}

//==============점프 시작 함수==========
void ATenCharacter::StartJump(const FInputActionValue& value)
{// Jump함수는 캐릭터가 기본 제공
    if (value.Get<bool>())
    {
        Jump();
    }
}

//============점프 끝 함수==============
void ATenCharacter::StopJump(const FInputActionValue& value)
{//StopJump함수도 캐릭터가 기본 제공
    if (value.Get<bool>())
    {
        StopJumping();
    }
}

//==============마우스 회전============
void ATenCharacter::Look(const FInputActionValue& value)
{//마우스의 X, Y 움직임을 2D축으로 가져옴
    FVector2D LookInput = value.Get<FVector2D>();
    //좌우 회전(X = Yaw , Y = Pitch) 
    AddControllerYawInput(LookInput.X);
    //상하 회전
    AddControllerPitchInput(LookInput.Y);
}
