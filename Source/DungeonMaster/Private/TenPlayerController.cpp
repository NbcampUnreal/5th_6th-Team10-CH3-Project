#include "TenPlayerController.h"
#include "EnhancedInputSubsystems.h"		//인핸스드 인풋 서브 시스템(로컬)

ATenPlayerController::ATenPlayerController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr)
{}

void ATenPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//현재 플레이어컨트롤러에 연결된 로컬플레이어 객체를 가져옴
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{	//로컬 플레이어에서 인핸스드 인풋 로컬 플레이어 서브시스템을 획득
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{	//서브 시스템을 통해 IMC를 활성화 하고 우선순위 0번 인덱스
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}