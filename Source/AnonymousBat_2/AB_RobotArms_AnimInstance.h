// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "AB_Pawn_To_AnimInst_Interface.h"
#include "Animation/AnimInstance.h"
#include "AB_RobotArms_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANONYMOUSBAT_2_API UAB_RobotArms_AnimInstance : public UAnimInstance, public IAB_Pawn_To_AnimInst_Interface
{
	GENERATED_BODY()

public:
	UAB_RobotArms_AnimInstance();
	virtual void NativeUpdateAnimation(float _DeltaSeconds) override;
	virtual void PrePushSoundCube_Implementation(const UPrimitiveComponent* _pComponent) override;
	virtual void PushSoundCube_Implementation(const UPrimitiveComponent* _pComponent) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Pawn, meta=(AllowPrivateAccess=true))
	FVector SoundCubeTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Pawn, meta=(AllowPrivateAccess=true))
	bool bIsMouseButtonDown_AnimInst;
};
