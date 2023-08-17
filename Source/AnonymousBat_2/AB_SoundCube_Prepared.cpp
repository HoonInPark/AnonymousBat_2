// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_SoundCube_Prepared.h"

#include "Kismet/KismetMathLibrary.h"

AAB_SoundCube_Prepared::AAB_SoundCube_Prepared()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshNum = 0;
	
	CubeSize = 30.0f;
	NumCubes = 10;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->SetMobility(EComponentMobility::Static);
	SetRootComponent(RootComponent);

	// Find and assign cube meshes
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder_0(
		TEXT("/Script/Engine.StaticMesh'/Game/_05_DancingCubes/Meshes/Actor01.Actor01'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder_1(
		TEXT("/Script/Engine.StaticMesh'/Game/_05_DancingCubes/Meshes/Actor02.Actor02'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder_2(
		TEXT("/Script/Engine.StaticMesh'/Game/_05_DancingCubes/Meshes/Actor03.Actor03'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder_3(
		TEXT("/Script/Engine.StaticMesh'/Game/_05_DancingCubes/Meshes/Actor04.Actor04'"));

	if (CubeMeshFinder_0.Succeeded() && CubeMeshFinder_1.Succeeded() && CubeMeshFinder_2.Succeeded() &&
		CubeMeshFinder_3.Succeeded())
	{
		pCubeMeshes.Add(CubeMeshFinder_0.Object);
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

				pCubeComponent->SetStaticMesh(pCubeMeshes[StaticMeshNum]);
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

void AAB_SoundCube_Prepared::Tick(float _DeltaSeconds)
{
	Super::Tick(_DeltaSeconds);
}

void AAB_SoundCube_Prepared::SoundCubeVisualizer_MouseButtonDown_Implementation(UPrimitiveComponent* _ClosestHit, AAB_Pawn* _pCaller)
{
	_ClosestHit->SetVisibility(false);

	for (const auto pEachCube : this->GetComponents())
	{
		pEachCube_SM = Cast<UStaticMeshComponent>(pEachCube);
		if (pEachCube_SM)
		{
			if (pEachCube_SM != _ClosestHit && pEachCube_SM->GetCollisionObjectType() != ECC_WorldStatic)
			{
				pEachCube_SM->SetVisibility(true);
				AB2LOG(Warning, TEXT("%s"), *pEachCube_SM->GetName());
			}
		}
	}
}

void AAB_SoundCube_Prepared::SoundCubeVisualizer_MouseButtonUp_Implementation(UPrimitiveComponent* _ClosestHit, AAB_Pawn* _pCaller)
{
	_ClosestHit->SetVisibility(true);
	_ClosestHit->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
}

void AAB_SoundCube_Prepared::SetStaticMeshNum(int32 NewStaticMeshNum)
{
	if (pCubeMeshes.Num() > NewStaticMeshNum)
	{
		StaticMeshNum = NewStaticMeshNum;
		for (const auto CubeComponent : GetComponentsByClass(UStaticMeshComponent::StaticClass()))
		{
			UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(CubeComponent);
			if (StaticMeshComponent && StaticMeshComponent != RootComponent)
				StaticMeshComponent->SetStaticMesh(pCubeMeshes[StaticMeshNum]);
		}
	}
}
