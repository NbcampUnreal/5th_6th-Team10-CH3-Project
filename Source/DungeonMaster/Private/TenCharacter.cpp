#include "TenCharacter.h"
#include "Camera/CameraComponent.h"											//������ ��, ī�޶� ������
#include "GameFramework/SpringArmComponent.h"						//������ ��, ī�޶� ������
#include "EnhancedInputComponent.h"												//UEnhancedInputComponent ��� ��									
#include "TenPlayerController.h"															//�÷��̾� ��Ʈ�ѷ� ���
#include "GameFramework/CharacterMovementComponent.h"		//�÷��̾� �����Ʈ �Լ���� ��

ATenCharacter::ATenCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// ĳ���Ͱ� ��Ʈ�ѷ� ȸ���� ������ ���� �ʵ��� ���� (ī�޶� ȸ��)
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//=========������ �� ===========

	//������ �� ������ ��Ʈ�� ����
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	//ĳ���Ϳ� ī�޶� ���� �Ÿ�
	SpringArmComp->TargetArmLength = 300.0f;
	//��Ʈ�ѷ� ȸ���� ���� ������ �ϵ� ȸ�� ����
	SpringArmComp->bUsePawnControlRotation = true;

	//=========ī�޶� ==========

	//ī�޶� ���� �� ������ �Ͽ� ����
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	//������ ���� ȸ���� �������� false;
	CameraComp->bUsePawnControlRotation = false;

	//==========ĳ���� ����============

	//ĳ���� HP
	MaxHealth = 100.f;
	Health = MaxHealth;
    //�̵� �ӵ�
    NormalSpeed = 600.0f;
    //ĳ���� �⺻ �ӵ� ����
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    // ========ĳ���� �̵� ������Ʈ ����=====
    // ĳ���Ͱ� �̵� �������� ȸ���ϵ��� ����.
    GetCharacterMovement()->bOrientRotationToMovement = true;
    // ĳ���� ȸ�� �ӵ��� �����մϴ�. (�ʴ� 540��)
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    // ĳ���Ͱ� ���߿����� �̵� �������� ȸ������ ���� (�ʿ信 ���� ����)
    GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false; //  false�� �⺻
}

// =========ĳ���� ���� ���� ===============
void ATenCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent�� ĳ����
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA�� �������� ���� ���� ���� ������Ʈ�ѷ��� ����� �� ��Ʈ�ѷ��� ĳ����
        if (ATenPlayerController* PlayerController = Cast<ATenPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // IA_Move �׼� Ű�� "Ű�� ������ �ִ� ����" Move() ȣ��
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ATenCharacter::Move);
            }

            //����
            if (PlayerController->JumpAction)
            {
                // IA_Jump �׼� Ű�� "Ű�� ������ �ִ� ����" StartJump() ȣ��
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ATenCharacter::StartJump);
                // IA_Jump �׼� Ű���� "���� �� ����" StopJump() ȣ��
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ATenCharacter::StopJump);
            }
            //��
            if (PlayerController->LookAction)
            {
                // IA_Look �׼� ���콺�� "������ ��" Look() ȣ��
                EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ATenCharacter::Look);
            }
        }
    }
}

//==============�̵� �Լ�===============
void ATenCharacter::Move(const FInputActionValue& value)
{
    FVector2D MovementVector = value.Get<FVector2D>();

    float ForwardValue = MovementVector.X; // X���� ��������
    float RightValue = MovementVector.Y;   // Y���� ����������
    //��Ʈ�ѷ��� �ִ��� üũ
    if (Controller != nullptr)
    {
        // ���� ��Ʈ�ѷ��� ȸ���� �����ɴϴ�.
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0); // Yaw ���� ���

        // ���� ���� ���
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        // ���� ���� ���
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // ����/�Ĺ� �̵�
        AddMovementInput(ForwardDirection, ForwardValue);
        // ��/���� �̵�
        AddMovementInput(RightDirection, RightValue);
    }
}

//==============���� ���� �Լ�==========
void ATenCharacter::StartJump(const FInputActionValue& value)
{// Jump�Լ��� ĳ���Ͱ� �⺻ ����
    if (value.Get<bool>())
    {
        Jump();
    }
}

//============���� �� �Լ�==============
void ATenCharacter::StopJump(const FInputActionValue& value)
{//StopJump�Լ��� ĳ���Ͱ� �⺻ ����
    if (value.Get<bool>())
    {
        StopJumping();
    }
}

//==============���콺 ȸ��============
void ATenCharacter::Look(const FInputActionValue& value)
{//���콺�� X, Y �������� 2D������ ������
    FVector2D LookInput = value.Get<FVector2D>();
    //�¿� ȸ��(X = Yaw , Y = Pitch) 
    AddControllerYawInput(LookInput.X);
    //���� ȸ��
    AddControllerPitchInput(LookInput.Y);
}
