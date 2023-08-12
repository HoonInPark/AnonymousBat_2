// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_RobotArms_AnimInstance.h"

UAB_RobotArms_AnimInstance::UAB_RobotArms_AnimInstance()
{
	bDoOnce = true;
	
	bIsIdle = true;
	bIsGrab = false;
}

void UAB_RobotArms_AnimInstance::NativeUpdateAnimation(float _DeltaSeconds)
{
	Super::NativeUpdateAnimation(_DeltaSeconds);
}

// 마우스 버튼이 눌리기 시작할 때부터 눌리고 있을 동안 계속 호출됨.
void UAB_RobotArms_AnimInstance::PrePushSoundCube_Implementation(const UPrimitiveComponent* _pComponent)
{
	SoundCubeTransform = _pComponent->GetComponentLocation();

	if (bDoOnce)
	{
		bIsIdle = false;
		bIsGrab = true;

		bDoOnce = false;
	}
}

void UAB_RobotArms_AnimInstance::PushSoundCube_Implementation(const UPrimitiveComponent* _pComponent)
{
	bIsIdle = true;
	bIsGrab = false;
	
	bDoOnce = true;
}
