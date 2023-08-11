// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_RobotArms_AnimInstance.h"

UAB_RobotArms_AnimInstance::UAB_RobotArms_AnimInstance()
{
	bIsMouseButtonDown_AnimInst = false;
}

void UAB_RobotArms_AnimInstance::NativeUpdateAnimation(float _DeltaSeconds)
{
	Super::NativeUpdateAnimation(_DeltaSeconds);
	
}

void UAB_RobotArms_AnimInstance::PrePushSoundCube_Implementation(const UPrimitiveComponent* _pComponent)
{
	SoundCubeTransform = _pComponent->GetComponentLocation();
}

void UAB_RobotArms_AnimInstance::PushSoundCube_Implementation(const UPrimitiveComponent* _pComponent)
{
	SoundCubeTransform = _pComponent->GetComponentLocation();
	
}
