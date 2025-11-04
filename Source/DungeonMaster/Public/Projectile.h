#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class DUNGEONMASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AProjectile();

protected:
	//투사체 충돌 영역
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComp;
	
	//투사체 이동로직
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;
	
	//발사체 대미지
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float Damage = 40.f;

	//충돌 시 호출 함수
	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComp,			//충돌 본체
		AActor* OtherActor,								//부딪힌 액터
		UPrimitiveComponent* OtherComp,	//대상
		FVector NormalImpulse,							//충돌 방향
		const FHitResult& Hit);							//충돌 상세 정보
};
