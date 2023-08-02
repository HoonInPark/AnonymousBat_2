// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AB_SoundCube_2.generated.h"

/// <summary>
/// 이 클래스는 사운드큐브를 일렬로 객체화한다.
///	스테틱메시를 가져와서 하나의 큐브를 메시컴포넌트로 하고, 여기에 숫자를 붙여서 구분한다.
/// </summary>

UCLASS()
class ANONYMOUSBAT_2_API AAB_SoundCube_2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAB_SoundCube_2();
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                           const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                         class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category=AB_SoundCube)
		TArray<UStaticMesh*> pCubeMeshes;
	UPROPERTY(EditAnywhere, Category=AB_SoundCube)
		float CubeSize;
	UPROPERTY(EditAnywhere, Category=AB_SoundCube)
		int32 NumCubes;
	UPROPERTY(EditAnywhere, Category=AB_SoundCube)
		UStaticMeshComponent* pCubeComponent;
	UPROPERTY(EditAnywhere, Category=AB_SoundCube)
	UStaticMeshComponent* _pSoundCube;

	void IncreaseCubeSize(float SizeMultiplier);
	void UpdateCubesScale();

};
