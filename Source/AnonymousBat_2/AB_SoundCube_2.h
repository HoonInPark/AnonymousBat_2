// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AB_Pawn_To_SoundCube_Interface.h"
#include "AB_SoundCube_2.generated.h"


/// <summary>
/// �� Ŭ������ ����ť�긦 �Ϸķ� ��üȭ�Ѵ�.
///	����ƽ�޽ø� �����ͼ� �ϳ��� ť�긦 �޽�������Ʈ�� �ϰ�, ���⿡ ���ڸ� �ٿ��� �����Ѵ�.
/// </summary>

UCLASS()
class ANONYMOUSBAT_2_API AAB_SoundCube_2 : public AActor, public IAB_Pawn_To_SoundCube_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAB_SoundCube_2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AB_SoundCube)
	TArray<UStaticMesh*> pCubeMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AB_SoundCube)
	float CubeSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AB_SoundCube)
	int32 NumCubes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AB_SoundCube)
	UStaticMeshComponent* pCubeComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AB_SoundCube)
	UStaticMeshComponent* _pSoundCube;
	
	virtual void MusicStart_Implementation() override;
};
