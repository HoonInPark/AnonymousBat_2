// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_SoundCube_Prepared_Spawner.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAB_SoundCube_Prepared_Spawner::AAB_SoundCube_Prepared_Spawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	const TArray<FSpawnTransform> TransformDataArray = {
		{
			{FVector(-1000.0f, 1800.0f, 1180.0f), FRotator(-90.0f, -90.0f, 65.0f)},
			{FVector(-680.0f, 2700.0f, 1250.0f), FRotator(-90.0f, 0.0f, -89.999999f)},
			{FVector(-910.0f, 940.0f, 1450.0f), FRotator(-85.0f, 90.0f, 250.0f)},
			{FVector(830.f,2800.f,830.f), FRotator(0.f,0.f,-84.999999f)}
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

	SpawnStaticMeshNum = 0;
	for (const auto SpawnTransform : SpawnTransforms)
	{
		SpawnSoundCubes(SpawnStaticMeshNum, SpawnTransform);
		SpawnStaticMeshNum++;
	}
}

// Called every frame
void AAB_SoundCube_Prepared_Spawner::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
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

			for (int32 i = 0; i < 60; i++)
			{
				TArray<UStaticMeshComponent*> StaticMeshComponents;
				for (UActorComponent* Component : pAB_SoundCube_Prepared->GetComponents())
				{
					UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Component);
					if (StaticMeshComponent != nullptr && StaticMeshComponent != RootComponent)
					{
						StaticMeshComponents.Add(StaticMeshComponent);
					}
				}

				if (StaticMeshComponents.Num() > 0)
				{
					const int32 RandomIndex = UKismetMathLibrary::RandomInteger(StaticMeshComponents.Num());
					UActorComponent* ComponentToDestroy = StaticMeshComponents[RandomIndex];
					ComponentToDestroy->DestroyComponent();
				}
			}
		}
	}
}
