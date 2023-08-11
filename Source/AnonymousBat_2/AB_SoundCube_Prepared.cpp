// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_SoundCube_Prepared.h"

AAB_SoundCube_Prepared::AAB_SoundCube_Prepared()
{
	PrimaryActorTick.bCanEverTick = false;

	CubeSize = 30.0f;
	NumCubes = 10;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->SetMobility(EComponentMobility::Static);
	SetRootComponent(RootComponent);

	// Find and assign cube meshes
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder_Default(
		TEXT("/Script/Engine.StaticMesh'/Game/_05_DancingCubes/Meshes/AB_Cube.AB_Cube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder_1(
		TEXT("/Script/Engine.StaticMesh'/Game/_05_DancingCubes/Meshes/Actor01_3.Actor01_3'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder_2(
		TEXT("/Script/Engine.StaticMesh'/Game/_05_DancingCubes/Meshes/Actor02_3.Actor02_3'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder_3(
		TEXT("/Script/Engine.StaticMesh'/Game/_05_DancingCubes/Meshes/Actor03_3.Actor03_3'"));

	if (CubeMeshFinder_Default.Succeeded() && CubeMeshFinder_1.Succeeded() && CubeMeshFinder_2.Succeeded() &&
		CubeMeshFinder_3.Succeeded())
	{
		pCubeMeshes.Add(CubeMeshFinder_Default.Object);
		pCubeMeshes.Add(CubeMeshFinder_1.Object);
		pCubeMeshes.Add(CubeMeshFinder_2.Object);
		pCubeMeshes.Add(CubeMeshFinder_3.Object);
	}

	if (NumCubes > 0 && pCubeMeshes.Num() > 0)
	{
		const float TotalLength = NumCubes * CubeSize;

		for (int32 j = 0; j < NumCubes; j++)
		{
			for (int32 i = 0; i < NumCubes; i++)
			{
				FString ComponentName = FString::Printf(TEXT("%d %d"), i, j);

				pCubeComponent = CreateDefaultSubobject<UStaticMeshComponent>(*ComponentName);
				pCubeComponent->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
				pCubeComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				pCubeComponent->SetupAttachment(RootComponent);

				pCubeComponent->SetRelativeLocation(FVector(i * CubeSize * 0.4f, j * CubeSize * 0.4f, 0.f));
				pCubeComponent->SetRelativeRotation(
					FRotator(FMath::FRandRange(-30.0f, 30.0f), FMath::FRandRange(-30.0f, 30.0f), 0.0f));
				pCubeComponent->SetRelativeScale3D(FVector(CubeSize / 250.0f));
				pCubeComponent->SetMobility(EComponentMobility::Static);

				pCubeComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
				pCubeComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				pCubeComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

				pCubeComponent->SetStaticMesh(pCubeMeshes[FMath::RandRange(1, pCubeMeshes.Num() - 1)]);
				// pCubeComponent->SetStaticMesh(pCubeMeshes[0]);
				pCubeComponent->SetVisibility(true);
			}
		}

		SetActorScale3D(FVector(TotalLength / 100.0f));
	}
}

void AAB_SoundCube_Prepared::BeginPlay()
{
	Super::BeginPlay();
}

void AAB_SoundCube_Prepared::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AAB_SoundCube_Prepared::SoundCubeVisualizer_MouseButtonDown_Implementation(UPrimitiveComponent* _ClosestHit)
{
	_ClosestHit->SetVisibility(false);

	for (const auto pEachCube : this->GetComponents())
	{
		pEachCube_SM = Cast<UStaticMeshComponent>(pEachCube);
		if (pEachCube_SM && pEachCube_SM != _ClosestHit && pEachCube_SM->GetCollisionObjectType() != ECC_WorldStatic)
		{
			pEachCube_SM->SetVisibility(true);
			AB2LOG(Warning, TEXT("%s"), *pEachCube_SM->GetName());
		}
	}
}

void AAB_SoundCube_Prepared::SoundCubeVisualizer_MouseButtonUp_Implementation(UPrimitiveComponent* _ClosestHit)
{
	_ClosestHit->SetVisibility(true);
	_ClosestHit->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
}
