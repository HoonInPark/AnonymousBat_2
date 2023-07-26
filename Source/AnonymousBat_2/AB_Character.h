// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "GameFramework/Character.h"
#include "AB_SoundCube_2.h"
#include "AB_Character.generated.h"

/// <summary>
/// Todo: ����Ʈ���̽��� �������� �� ������ �ν��� �ϰ� ������ �÷��̾ ���� �ȿ� �������� �����Ǵ� ���� ���� �� �ִ�.
///	Todo: �׸��� �̰� �����ϴ� �������� ����Ʈ���̽��� ���� ���̸� ���� �� ����!
/// </summary>

UCLASS()
class ANONYMOUSBAT_2_API AAB_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAB_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* _pPlayerInputComponent) override;

private:
	// Set up parameters for getting the player viewport
	bool bIsEKeyDown;
	float Reach;

	FVector SweepStartPt;
	FVector SweepEndPt;
	TArray<FHitResult> HitResults;

	TArray<FString> CubeNames_Hit;
	TArray<FString> CubeNames_Actor;

	void PushSoundCube();
	void PrePushSoundCube();
	TArray<FHitResult> SweepInRange();
	bool IsGrounded(const UPrimitiveComponent* _pCubeComponent);

public:
	UPROPERTY(EditAnywhere, Category=AB_Character)
	FVector PlayerViewPtLoc;
	UPROPERTY(EditAnywhere, Category=AB_Character)
	FRotator PlayerViewPtRot;
	UPROPERTY()
	AAB_SoundCube_2* pAB_SoundCube;
	UPROPERTY()
	UMaterialInstanceDynamic* SoundCubeMatInstDynamic;
	UPROPERTY()
	UPrimitiveComponent* pCubeComponent_Hit;
	// UPROPERTY(EditAnywhere)
	// class UStaticMeshComponent* DynamicMaterialMesh;
};
