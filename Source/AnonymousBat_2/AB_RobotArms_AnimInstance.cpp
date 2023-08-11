// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_RobotArms_AnimInstance.h"

UAB_RobotArms_AnimInstance::UAB_RobotArms_AnimInstance()
{
	bDoOnce = true;
	bFlipFlop = true;
	bIsIdle = true;
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
		// ���� ��� ��������� ��� ����.
		StretchHand();
		bDoOnce = false;
	}
}

void UAB_RobotArms_AnimInstance::PushSoundCube_Implementation(const UPrimitiveComponent* _pComponent)
{
	// ���� ��� ����������� ����� ����.
	StretchHand();
	bDoOnce = true;
}

void UAB_RobotArms_AnimInstance::StretchHand()
{
	if (bFlipFlop)
	{
		bIsIdle = false;
		bFlipFlop = false;
	}
	else
	{
		bIsIdle = true;
		bFlipFlop = true;
	}
}
