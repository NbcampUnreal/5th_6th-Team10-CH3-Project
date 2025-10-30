// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PortalBase.generated.h"

class USphereComponent;

UCLASS()
class DUNGEONMASTER_API APortalBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortalBase();

protected:
	// 포탈 유형(타입)을 편집 가능하게 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	FName PortalType;

	// 루트 컴포넌트 (씬)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PortalComponent")
	USceneComponent* Scene;
	// 충돌 컴포넌트 (플레이어 진입 범위 감지용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PortalComponent")
	USphereComponent* Collision;
	// 포탈 시각 표현용 스태틱 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PortalComponent")
	UStaticMeshComponent* StaticMesh;

	// 플레이어가 이 아이템의 범위에 들어왔을 때 호출
	UFUNCTION()
	void OnPortalOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// 플레이어가 이 아이템의 범위를 벗어났을 때 호출
	UFUNCTION()
	void OnPortalEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	virtual void MoveLevel();
};

