// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAB_SoundCube_Prepared_v3.h"

AMyAB_SoundCube_Prepared_v3::AMyAB_SoundCube_Prepared_v3()
{
}

void AMyAB_SoundCube_Prepared_v3::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnCubes(2);
}
