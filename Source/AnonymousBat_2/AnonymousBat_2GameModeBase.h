// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AnonymousBat_2.h"
#include "AB_Pawn.h"
#include "GameFramework/GameModeBase.h"
#include "AnonymousBat_2GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ANONYMOUSBAT_2_API AAnonymousBat_2GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	AAnonymousBat_2GameModeBase();

public:
	virtual void PostLogin(APlayerController* _NewPlayer) override;
};
