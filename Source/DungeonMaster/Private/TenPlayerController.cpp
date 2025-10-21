#include "TenPlayerController.h"
#include "EnhancedInputSubsystems.h"		//���ڽ��� ��ǲ ���� �ý���(����)

ATenPlayerController::ATenPlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr)
{}

void ATenPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//���� �÷��̾���Ʈ�ѷ��� ����� �����÷��̾� ��ü�� ������
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{	//���� �÷��̾�� ���ڽ��� ��ǲ ���� �÷��̾� ����ý����� ȹ��
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{	//���� �ý����� ���� IMC�� Ȱ��ȭ �ϰ� �켱���� 0�� �ε���
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}