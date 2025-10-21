#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TenCharacter.generated.h"

class USpringArmComponent;    //������ �� ������Ʈ ���� ����
class UCameraComponent;			 //ī�޶� ������Ʈ ���� ����
struct FInputActionValue;			 //Enhanced Input���� �׼� �� ���� �� ����ϴ� ����ü 

UCLASS()
class DUNGEONMASTER_API ATenCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ATenCharacter();

protected:

	//������ �� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	//ī�޶� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	//===============�̵��� ����=================
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);

	//============ ȣ���============
	
	//���� ü�� ��ȯ
	//UFUNCTION(BlueprintCallable, Category = "Stats")
	//float GetHealth() const;

	////ĳ���Ϳ��� ������ ���� �Լ�
	//UFUNCTION(BlueprintCallable, Category = "Stats")
	//void ApplyDamageToCharacter(float DamageAmount);

	// ============ĳ���� ����=========

	//�ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;

	//���� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health;

	//�̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float NormalSpeed;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
