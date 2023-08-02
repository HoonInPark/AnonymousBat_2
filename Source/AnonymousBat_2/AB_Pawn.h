// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "GameFramework/Pawn.h"
#include "AB_SoundCube_2.h"
#include "AB_Pawn.generated.h"

UCLASS()
class ANONYMOUSBAT_2_API AAB_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAB_Pawn();

	float Speed_Max{4000.f};
	float Speed_min{500.f};
	float CurrentSpeed_Forward{0.f};
	float CurrentSpeed_Right{0.f};

	float RateMultiplierRoll{200.f};
	float RateMultiplierPitch{200.f};

	float CurrentSpeed_Yaw;
	float CurrentSpeed_Pitch;
	float CurrentSpeed_Roll;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlaneMove_Forward(float _Value);
	void PlaneMove_Right(float _Value);
	
	void ProcessMouseInputY(float _Value);
	void ProcessMouseInputX(float _Value);

	void ProcessRoll(float _Value);
	void ProcessPitch(float _Value);

public:
	// Called every frame
	virtual void Tick(float _DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* _NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadOnly, Category=Collision)
	UCapsuleComponent* pCapsule;
	UPROPERTY(BlueprintReadOnly, Category=Collision)
	USkeletalMeshComponent* pMesh;
	UPROPERTY(BlueprintReadOnly, Category=Collision)
	UCameraComponent* pCamera;

private:
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

private:
	UPROPERTY()
	class AAB_PlayerController* AB_PlayerController;
};
