// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "AB_Pawn.h"
#include "GameFramework/PlayerController.h"
#include "AB_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ANONYMOUSBAT_2_API AAB_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* _InPawn) override;
};
