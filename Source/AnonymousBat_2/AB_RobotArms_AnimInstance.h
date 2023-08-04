// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AnonymousBat_2.h"
#include "Animation/AnimInstance.h"
#include "AB_RobotArms_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ANONYMOUSBAT_2_API UAB_RobotArms_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAB_RobotArms_AnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, meta=(AllowPrivateAccess=true))
	FVector SoundCubeTransform;
};
