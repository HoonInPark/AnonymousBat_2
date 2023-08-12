// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "AB_Pawn_To_SoundCube_Interface.h"
#include "MyActor2.generated.h"

/**
 * 
 */
UCLASS()
class ANONYMOUSBAT_2_API AMyActor2 : public AActor, public IAB_Pawn_To_SoundCube_Interface
{
	GENERATED_BODY()

public:
	AMyActor2();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AB_SoundCube_Prepared)
	TArray<UStaticMesh*> pCubeMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AB_SoundCube_Prepared)
	float CubeSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AB_SoundCube_Prepared)
	int32 NumCubes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AB_SoundCube_Prepared)
	UStaticMeshComponent* pCubeComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AB_SoundCube_Prepared)
	UStaticMeshComponent* pEachCube_SM;
	
	virtual void SoundCubeVisualizer_MouseButtonDown_Implementation(UPrimitiveComponent* _ClosestHit) override;
	virtual void SoundCubeVisualizer_MouseButtonUp_Implementation(UPrimitiveComponent* _ClosestHit) override;
};