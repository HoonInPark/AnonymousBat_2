// Copyright Epic Games, Inc. All Rights Reserved.


#include "AnonymousBat_2GameModeBase.h"

#include "AB_Pawn.h"
#include "AB_PlayerController.h"

AAnonymousBat_2GameModeBase::AAnonymousBat_2GameModeBase()
{
	DefaultPawnClass = AAB_Pawn::StaticClass();
	PlayerControllerClass = AAB_PlayerController::StaticClass();
}

void AAnonymousBat_2GameModeBase::PostLogin(APlayerController* _NewPlayer)
{
	AB2LOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(_NewPlayer);
	AB2LOG(Warning, TEXT("PostLogin Complete"));
}
