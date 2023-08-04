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
		// 스윕을 해서 컴포넌트가 SetVisibility(true)가 되려는 컴포넌트의 FVevtor값을 가져와서 SoundCubeTransform에 넣는다.
		AB2LOG(Warning, TEXT("%s"), *SoundCubeTransform.ToString());
		
	}
}
