// Copyright Epic Games, Inc. All Rights Reserved.


#include "AnonymousBat_2GameModeBase.h"

#include "AB_PlayerController.h"

AAnonymousBat_2GameModeBase::AAnonymousBat_2GameModeBase()
{
	DefaultPawnClass = AAB_Character::StaticClass();
	PlayerControllerClass = AAB_PlayerController::StaticClass();
}
