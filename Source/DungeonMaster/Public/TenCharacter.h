#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TenCharacter.generated.h"

class USpringArmComponent;    //스프링 암 컴포넌트 전방 선언
class UCameraComponent;			 //카메라 컴포넌트 전방 선언
struct FInputActionValue;			 //Enhanced Input에서 액션 값 받을 때 사용하는 구조체 

UCLASS()
class DUNGEONMASTER_API ATenCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ATenCharacter();

protected:

	//스프링 암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	//카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	//===============이동과 시점=================
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);

	//============ 호출용============
	
	//현재 체력 반환
	//UFUNCTION(BlueprintCallable, Category = "Stats")
	//float GetHealth() const;

	////캐릭터에게 데미지 적용 함수
	//UFUNCTION(BlueprintCallable, Category = "Stats")
	//void ApplyDamageToCharacter(float DamageAmount);

	// ============캐릭터 정보=========

	//최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth;

	//현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health;

	//이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float NormalSpeed;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
