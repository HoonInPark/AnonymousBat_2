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
	
}

void UAB_RobotArms_AnimInstance::PrePushSoundCube_Implementation(UPrimitiveComponent* pComponent)
{
	if (OwningPawn)
	{
		AB2LOG(Warning, TEXT("%s"), *pComponent->GetName());
	}
}

void UAB_RobotArms_AnimInstance::PushSoundCube_Implementation(UPrimitiveComponent* pComponent)
{
	if (OwningPawn)
	{
	}
}
