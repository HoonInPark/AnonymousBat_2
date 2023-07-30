// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "GameFramework/Character.h"
#include "AB_SoundCube_2.h"
#include "AB_Character.generated.h"

/// <summary>
/// Todo: 라인트레이스가 일정정도 먼 곳부터 인식을 하게 만들어야 플레이어가 액터 안에 들어오도록 스폰되는 일을 막을 수 있다.
///	Todo: 그리고 이걸 구현하는 과정에서 라인트레이스가 눈에 보이면 좋을 것 같다!
/// </summary>

UCLASS()
class ANONYMOUSBAT_2_API AAB_Character : public ACharacter
{
	GENERATED_BODY()

	float Acceleration{30.f};
	float speed_Max{4000.f};
	float speed_min{500.f};

	float currentSpeed_Forward{500.f};
	
	float currentSpeed_Yaw;
	float currentSpeed_Pitch;
	float currentSpeed_Roll;
	
public:
	// Sets default values for this character's properties
	AAB_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ProcessKeyPitch(float _Rate);
	void ProcessKeyRoll(float _Rate);
	
	void ProcessMouseInputY(float _Value);
	void ProcessMouseInputX(float _Value);
	
	void ProcessRoll(float _Value);
	void ProcessPitch(float _Value);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* _pPlayerInputComponent) override;

private:
	// Set up parameters for getting the player viewport
	bool bIsEKeyDown;
	FVector PlayerViewPtLoc;
	FRotator PlayerViewPtRot;
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
	UPROPERTY()
	AAB_SoundCube_2* pAB_SoundCube;
	UPROPERTY()
	UMaterialInstanceDynamic* SoundCubeMatInstDynamic;
	// UPROPERTY(EditAnywhere)
	// class UStaticMeshComponent* DynamicMaterialMesh;
};
