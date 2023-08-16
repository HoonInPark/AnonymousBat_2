// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AB_SoundCube_Prepared.h"
#include "MyAB_SoundCube_Prepared_v2.generated.h"

/**
 * 
 */
UCLASS()
class ANONYMOUSBAT_2_API AMyAB_SoundCube_Prepared_v2 : public AAB_SoundCube_Prepared
{
	GENERATED_BODY()

public:
	AMyAB_SoundCube_Prepared_v2();
	virtual void BeginPlay() override;
};
