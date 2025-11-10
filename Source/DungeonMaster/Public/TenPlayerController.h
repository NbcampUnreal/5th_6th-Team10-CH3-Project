#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TenPlayerController.generated.h"

class UInputMappingContext; // IMC전방 선언
class UInputAction; //IA 관련 전방 선언

UCLASS()
class DUNGEONMASTER_API ATenPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	//생성자
	ATenPlayerController();

	//IMC 세팅
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	//IA_Move 지정 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	//IA_Jump 지정 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	// IA_Look 지정 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* EquipRangedAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* EquipMeleeAction;

	//회피 입력 액션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DodgeAction;

	virtual void BeginPlay() override;
};
