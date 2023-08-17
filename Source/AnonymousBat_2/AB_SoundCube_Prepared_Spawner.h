// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "GameFramework/Actor.h"
#include "AB_SoundCube_Prepared.h"
#include "AB_SoundCube_Prepared_Spawner.generated.h"

USTRUCT()
struct FSpawnTransform
{
	GENERATED_BODY()
	
	FVector Location;
	FRotator Rotation;
};

UCLASS()
class ANONYMOUSBAT_2_API AAB_SoundCube_Prepared_Spawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAB_SoundCube_Prepared_Spawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float _DeltaTime) override;

private:
	TArray<FTransform> SpawnTransforms;
	void SpawnSoundCubes(int32 _SpawnStaticMeshNum, const FTransform& _SpawnTransform);

public:
	int32 SpawnStaticMeshNum;
};
