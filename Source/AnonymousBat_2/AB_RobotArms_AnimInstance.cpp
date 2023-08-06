// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_RobotArms_AnimInstance.h"
#include "AB_Pawn.h"

UAB_RobotArms_AnimInstance::UAB_RobotArms_AnimInstance()
{
	SoundCubeTransform = FVector(0.f, 0.f, 50.f);
}

void UAB_RobotArms_AnimInstance::NativeUpdateAnimation(float _DeltaSeconds)
{
	Super::NativeUpdateAnimation(_DeltaSeconds);

	OwningPawn = Cast<AAB_Pawn>(TryGetPawnOwner());
	
	// 스윕을 해서 컴포넌트가 SetVisibility(true)가 되려는 컴포넌트의 FVevtor값을 가져와서 SoundCubeTransform에 넣는다.
	// SoundCubeTransform = PushSoundCube_Implementation()->GetActorTransform();
}

AAB_SoundCube_2* UAB_RobotArms_AnimInstance::PrePushSoundCube_Implementation()
{
	if (OwningPawn)
	{
		AB2LOG(Warning, TEXT("%s"), *OwningPawn->PrePushSoundCube()->GetName())
		return OwningPawn->PrePushSoundCube();
	}

	return nullptr;
}

AAB_SoundCube_2* UAB_RobotArms_AnimInstance::PushSoundCube_Implementation()
{
	if (OwningPawn)
	{
		AB2LOG(Warning, TEXT("%s"), *OwningPawn->PushSoundCube()->GetName())
		return OwningPawn->PrePushSoundCube();
	}

	return nullptr;
}
