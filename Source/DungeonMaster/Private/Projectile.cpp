#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	//충돌 범위 와 컴포넌트 생성
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.f);		//충돌 범위
	CollisionComp->SetCollisionProfileName("Projectile");	
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	RootComponent = CollisionComp;
	
	//투사체 이동 컴포넌트
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 9000.f;	//초기속도
	ProjectileMovement->MaxSpeed = 9000.f; //최대 속도
	ProjectileMovement->bRotationFollowsVelocity = true;	//이동 방향에 따라 회전하도록 설정
	ProjectileMovement->bShouldBounce = false; //팅기지 않도록 설정
	ProjectileMovement->ProjectileGravityScale = 0.f;//중력 영향 0

	//투사체 생명 주기
	InitialLifeSpan = 2.0f; //2초 후 자동 사라짐
}

//충돌 시 호출 함수
void AProjectile::OnHit(
	UPrimitiveComponent* HitComp,			//충돌한 투사체
	AActor* OtherActor,								//부딪힌 대상 액터
	UPrimitiveComponent* OtherComp,	//대상의 컴포넌트
	FVector NormalImpulse,							//충돌 방향 백터
	const FHitResult& Hit
)
{//다른 액터와 충돌 처리
	if (OtherActor &&OtherActor != this)
	{
		UGameplayStatics::ApplyPointDamage(
			OtherActor,									//대미지 받을 대상
			Damage,											//투사체 공격력
			GetVelocity().GetSafeNormal(),				// 투사체 방향
			Hit,
			nullptr,											//공격자는 컨트롤러 없으면 null
			this,													//대미지를 준 액터(캐릭터)
			UDamageType::StaticClass()		//대미지 타입 기본형
			);
		Destroy();
	}
}




