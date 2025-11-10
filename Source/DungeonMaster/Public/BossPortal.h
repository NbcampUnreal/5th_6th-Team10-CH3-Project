// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PortalBase.h"
#include "BossPortal.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMASTER_API ABossPortal : public APortalBase
{
	GENERATED_BODY()
	
public:
	ABossPortal();

protected:
	virtual void MoveLevel() override;

};
