// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PortalBase.h"
#include "DungeonFinishPortal.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMASTER_API ADungeonFinishPortal : public APortalBase
{
	GENERATED_BODY()
	
public:
	ADungeonFinishPortal();

protected:
	virtual void MoveLevel() override;

};
