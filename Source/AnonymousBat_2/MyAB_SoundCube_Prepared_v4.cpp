// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAB_SoundCube_Prepared_v4.h"

AMyAB_SoundCube_Prepared_v4::AMyAB_SoundCube_Prepared_v4()
{
}

void AMyAB_SoundCube_Prepared_v4::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnCubes(3);
}
