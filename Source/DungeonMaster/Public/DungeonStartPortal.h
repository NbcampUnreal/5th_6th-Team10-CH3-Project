// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PortalBase.h"
#include "DungeonStartPortal.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMASTER_API ADungeonStartPortal : public APortalBase
{
	GENERATED_BODY()

public:
	ADungeonStartPortal();

protected:
	virtual void MoveLevel() override;

};
