// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAB_SoundCube_Prepared_v2.h"

AMyAB_SoundCube_Prepared_v2::AMyAB_SoundCube_Prepared_v2()
{
}

void AMyAB_SoundCube_Prepared_v2::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnCubes(1);
}
