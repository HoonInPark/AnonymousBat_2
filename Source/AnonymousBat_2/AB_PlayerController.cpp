// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_PlayerController.h"

void AAB_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AB2LOG_S(Warning);
}

void AAB_PlayerController::OnPossess(APawn* _InPawn)
{
	AB2LOG_S(Warning);
	Super::OnPossess(_InPawn);
}
