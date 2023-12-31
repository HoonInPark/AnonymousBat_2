// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "GameFramework/Pawn.h"
#include "AB_SoundCube_2.h"
#include "AB_SoundCube_Prepared.h"
#include "AB_Pawn_To_AnimInst_Interface.h"
#include "AB_Pawn_To_SoundCube_Interface.h"
#include "AB_Pawn.generated.h"

class USoundCue;
class AAB_SoundCube_2;

/*
 *<현재 들고 있는 큐브에 할당된 정보들>
 *메시의 형태
 *할당된 사운드 모듈
 */

UCLASS()
class ANONYMOUSBAT_2_API AAB_Pawn : public APawn, public IAB_Pawn_To_AnimInst_Interface,
                                    public IAB_Pawn_To_SoundCube_Interface
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnonymousBat)
	UStaticMeshComponent* pBodyMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnonymousBat)
	UCameraComponent* pCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnonymousBat)
	USkeletalMeshComponent* pSkeletalMesh_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnonymousBat)
	USkeletalMeshComponent* pSkeletalMesh_L;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnonymousBat)
	UAnimInstance* pAnimInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnonymousBat)
	bool bIsMouseButtonDown;

private:

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

	void CallPrePushSoundCube();
	void CallPushSoundCube();

	virtual void PrePushSoundCube_Implementation(const UPrimitiveComponent* _pComponent) override;
	virtual void PushSoundCube_Implementation(const UPrimitiveComponent* _pComponent) override;
	virtual void SoundCubeVisualizer_MouseButtonDown_Implementation(UPrimitiveComponent* _ClosestHit, AAB_Pawn* _pCaller) override;
	virtual void SoundCubeVisualizer_MouseButtonUp_Implementation(UPrimitiveComponent* _ClosestHit, AAB_Pawn* _pCaller) override;

	virtual void MusicStart_Implementation() override;
	void PlayRandomSoundFromCategory(int32 _CategoryNum);

	TArray<FHitResult> SweepInRange();
	bool IsGrounded(const UPrimitiveComponent* _pCubeComponent);

public:
	UPROPERTY()
	AAB_SoundCube_2* pAB_SoundCube;
	UPROPERTY()
	AAB_SoundCube_Prepared* pAB_SoundCube_Prepared;
	UPROPERTY()
	UPrimitiveComponent* pClosestHitCube;
	UPROPERTY()
	TMap<UPrimitiveComponent*, UStaticMesh*> CubeMeshMap;

private:
	UPROPERTY()
	class AAB_PlayerController* AB_PlayerController;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnonymousBat)
	USkeletalMeshComponent* pSoundCubeHeld;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnonymousBat)
	TArray<USkeletalMesh*> pCubeMeshesHeld;
	
private:
	FName SoundCubeHeldSocket;
	float AttachDelay;
	FTimerHandle TimerHandle_AttachDelay;
	bool bShouldAttach;

	TArray<FString> SoundPaths_Bass;
	TArray<FString> SoundPaths_Drum;
	TArray<FString> SoundPaths_Guitar;
	TArray<FString> SoundPaths_Piano;

	TArray<TArray<FString>*> ArrOfArr;
	
	void AttachMeshToSocket();
	void AttachMeshWithDelay();

	
	UPROPERTY()
	UAudioComponent* CurrentAudioComponent;

	UFUNCTION(BlueprintCallable)
	void PlayMusic(USoundWave* SoundWave);
	UFUNCTION(BlueprintCallable)
	void StopMusic();
	UFUNCTION(BlueprintCallable)
	void OnAudioFinished();
	
};
