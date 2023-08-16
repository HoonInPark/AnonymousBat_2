// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_RobotArms_AnimInstance.h"

UAB_RobotArms_AnimInstance::UAB_RobotArms_AnimInstance()
{
	SoundCubeTransform = FVector::Zero();
	bDoOnce = true;

	bIsStretch1 = false;
	bIsStretch2 = false;
}

void UAB_RobotArms_AnimInstance::NativeUpdateAnimation(float _DeltaSeconds)
{
	Super::NativeUpdateAnimation(_DeltaSeconds);
}

// ���콺 ��ư�� ������ ������ ������ ������ ���� ���� ��� ȣ���.
void UAB_RobotArms_AnimInstance::PrePushSoundCube_Implementation(const UPrimitiveComponent* _pComponent)
{
	SoundCubeTransform = _pComponent->GetComponentLocation();
	if (bDoOnce)
	{
		bIsStretch1 = true;
		bIsStretch2 = false;
		bDoOnce = false;
	}
}

void UAB_RobotArms_AnimInstance::PushSoundCube_Implementation(const UPrimitiveComponent* _pComponent)
{
	SoundCubeTransform = _pComponent->GetComponentLocation();
	bIsStretch1 = false;
	bIsStretch2 = true;
	bDoOnce = true;
}
