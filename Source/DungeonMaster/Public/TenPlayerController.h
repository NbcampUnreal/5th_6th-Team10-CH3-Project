#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TenPlayerController.generated.h"

class UInputMappingContext; // IMC���� ����
class UInputAction; //IA ���� ���� ����

UCLASS()
class DUNGEONMASTER_API ATenPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ATenPlayerController();

	//IMC ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	//IA_Move ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	//IA_Jump ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	// IA_Look ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* EquipRangedAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* EquipMeleeAction;

	virtual void BeginPlay() override;
};
