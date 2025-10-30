// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PortalBase.h"
#include "ClearPortal.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMASTER_API AClearPortal : public APortalBase
{
	GENERATED_BODY()

public:
	AClearPortal();

protected:
	virtual void MoveLevel() override;
	
};
