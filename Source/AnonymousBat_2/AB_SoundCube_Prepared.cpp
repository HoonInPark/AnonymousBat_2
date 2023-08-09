// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_SoundCube_Prepared.h"

AAB_SoundCube_Prepared::AAB_SoundCube_Prepared() : AAB_SoundCube_2()
{
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
}
