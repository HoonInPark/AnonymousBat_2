// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AB_SoundCube_Prepared.h"
#include "MyAB_SoundCube_Prepared_v1.generated.h"

/**
 * 
 */
UCLASS()
class ANONYMOUSBAT_2_API AMyAB_SoundCube_Prepared_v1 : public AAB_SoundCube_Prepared
{
	GENERATED_BODY()

	AMyAB_SoundCube_Prepared_v1();
	virtual void BeginPlay() override;
	
};
