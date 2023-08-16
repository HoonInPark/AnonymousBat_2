// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_SoundCube_Prepared_Spawner.h"

// Sets default values
AAB_SoundCube_Prepared_Spawner::AAB_SoundCube_Prepared_Spawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const TArray<FSpawnTransform> TransformDataArray = {
		{
			{FVector(-1000.0f, 1800.0f, 1180.0f), FRotator(-90.0f, -90.0f, 65.0f)},
			{FVector(-680.0f, 2700.0f, 1250.0f), FRotator(-90.0f, 0.0f, -89.999999f)},
			{FVector(-910.0f, 940.0f, 1450.0f), FRotator(-85.0f, 90.0f, 250.0f)},
			{FVector(2120.0f, -160.0f, 1310.0f), FRotator(-59.442661f, 234.962055f, 268.112969f)}
		}
	};
	
	for (const FSpawnTransform& TransformData : TransformDataArray)
	{
		FTransform Transform(TransformData.Rotation, TransformData.Location);
		SpawnTransforms.Add(Transform);
	}
}

// Called when the game starts or when spawned
void AAB_SoundCube_Prepared_Spawner::BeginPlay()
{
	Super::BeginPlay();

	int32 SpawnStaticMeshNum = 0;
	for (const auto SpawnTransform : SpawnTransforms)
	{
		SpawnSoundCubes(SpawnStaticMeshNum, SpawnTransform);
		SpawnStaticMeshNum++;
	}
}

// Called every frame
void AAB_SoundCube_Prepared_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAB_SoundCube_Prepared_Spawner::SpawnSoundCubes(int32 _SpawnStaticMeshNum, const FTransform& _SpawnTransform)
{
	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		if (AAB_SoundCube_Prepared* pAB_SoundCube_Prepared = GetWorld()->SpawnActor<AAB_SoundCube_Prepared>(
			AAB_SoundCube_Prepared::StaticClass(), _SpawnTransform, SpawnParams))
		{
			pAB_SoundCube_Prepared->SetStaticMeshNum(_SpawnStaticMeshNum);
		}
	}
}
