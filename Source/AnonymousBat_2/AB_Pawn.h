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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float Acceleration{30.f};
	float speed_Max{4000.f};
	float speed_min{500.f};

	float RateMultiplierRoll{200.f};
	float RateMultiplierPitch{200.f};
	float currentSpeed_Forward{500.f};
	
	float currentSpeed_Yaw;
	float currentSpeed_Pitch;
	float currentSpeed_Roll;

	void ProcessKeyInputForward(float _Value);
	void ProcessKeyInputRight(float _Value);
	
	void ProcessMouseInputY(float _Value);
	void ProcessMouseInputX(float _Value);
	
	void ProcessRoll(float _Value);
	void ProcessPitch(float _Value);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
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
