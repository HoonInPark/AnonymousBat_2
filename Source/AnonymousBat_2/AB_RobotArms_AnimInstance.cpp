// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_RobotArms_AnimInstance.h"

#include "AB_Pawn.h"

UAB_RobotArms_AnimInstance::UAB_RobotArms_AnimInstance()
{
	SoundCubeTransform = FVector(0.f, 0.f, 0.f);
}

void UAB_RobotArms_AnimInstance::NativeUpdateAnimation(float _DeltaSeconds)
{
	Super::NativeUpdateAnimation(_DeltaSeconds);


	// ������ �ؼ� ������Ʈ�� SetVisibility(true)�� �Ƿ��� ������Ʈ�� FVevtor���� �����ͼ� SoundCubeTransform�� �ִ´�.
	
}

void UAB_RobotArms_AnimInstance::PrePushSoundCube_Implementation()
{
}

void UAB_RobotArms_AnimInstance::PushSoundCube_Implementation()
{
}
