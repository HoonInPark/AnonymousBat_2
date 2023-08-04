// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_RobotArms_AnimInstance.h"

UAB_RobotArms_AnimInstance::UAB_RobotArms_AnimInstance()
{
	SoundCubeTransform = FVector(0.f, 0.f, 0.f);
}

void UAB_RobotArms_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		// ������ �ؼ� ������Ʈ�� SetVisibility(true)�� �Ƿ��� ������Ʈ�� FVevtor���� �����ͼ� SoundCubeTransform�� �ִ´�.
		AB2LOG(Warning, TEXT("%s"), *SoundCubeTransform.ToString());
		
	}
}
