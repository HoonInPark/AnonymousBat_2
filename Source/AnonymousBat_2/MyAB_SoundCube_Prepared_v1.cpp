// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAB_SoundCube_Prepared_v1.h"

AMyAB_SoundCube_Prepared_v1::AMyAB_SoundCube_Prepared_v1()
{
}

void AMyAB_SoundCube_Prepared_v1::BeginPlay()
{
	Super::BeginPlay();

	SpawnCubes(0);
}
