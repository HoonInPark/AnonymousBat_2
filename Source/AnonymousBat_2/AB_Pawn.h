// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "GameFramework/Pawn.h"
#include "AB_SoundCube_2.h"
#include "AB_Pawn_To_AnimInst_Interface.h"
#include "AB_Pawn.generated.h"

class AAB_SoundCube_2;

UCLASS()
class ANONYMOUSBAT_2_API AAB_Pawn : public APawn, public IAB_Pawn_To_AnimInst_Interface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAB_Pawn();

	float Speed_Max{4000.f};
	float Speed_min{500.f};
	float CurrentSpeed_Forward{1.f};
	float CurrentSpeed_Right{0.f};

	float RateMultiplierRoll{1.f};
	float RateMultiplierYaw{200.f};
	float RateMultiplierPitch{200.f};

	float CurrentSpeed_Roll{0.f};
	float CurrentSpeed_Pitch{0.f};
	float CurrentSpeed_Yaw{0.f};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlaneMove_Forward(float _Value);
	void PlaneMove_Right(float _Value);
	
	void ProcessYaw(float _Value);
	void ProcessPitch(float _Value);

public:
	// Called every frame
	virtual void Tick(float _DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* _NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category=Collision)
	UCapsuleComponent* pCapsule;
	UPROPERTY(EditAnywhere, Category=Collision)
	UStaticMeshComponent* pMesh;
	UPROPERTY(EditAnywhere, Category=Collision)
	UCameraComponent* pCamera;

private:
	bool bIsEKeyDown;
	FVector PlayerViewPtLoc;
	FRotator PlayerViewPtRot;
	float Reach;

	FVector SweepStartPt;
	FVector SweepEndPt;
	TArray<FHitResult> Hit_pressed;
	TArray<FHitResult> Hit_released;
	FHitResult ClosestHitResult;
	
	TArray<FString> CubeNames_Hit;
	TArray<FString> CubeNames_Actor;

	void CallPrePushSoundCube_Implementation();
	void CallPushSoundCube_Implementation();
	
	virtual void PrePushSoundCube_Implementation(AAB_SoundCube_2* _SoundCube) override;
	virtual void PushSoundCube_Implementation(AAB_SoundCube_2* SoundCube) override;

	TArray<FHitResult> SweepInRange();
	bool IsGrounded(const UPrimitiveComponent* _pCubeComponent);
	void MusicStart();

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


