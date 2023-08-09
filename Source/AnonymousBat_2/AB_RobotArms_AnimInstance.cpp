// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_RobotArms_AnimInstance.h"
#include "AB_Pawn.h"

UAB_RobotArms_AnimInstance::UAB_RobotArms_AnimInstance()
{
	bIsGonnaPush = false;
}

void UAB_RobotArms_AnimInstance::NativeUpdateAnimation(float _DeltaSeconds)
{
	Super::NativeUpdateAnimation(_DeltaSeconds);

	auto OwningPawn = Cast<AAB_Pawn>(TryGetPawnOwner());
}

void UAB_RobotArms_AnimInstance::PrePushSoundCube_Implementation(const UPrimitiveComponent* _pComponent)
{
	if (const auto OwningPawn = Cast<AAB_Pawn>(TryGetPawnOwner()))
	{
		if(FVector::Dist(OwningPawn->GetActorLocation(), SoundCubeTransform) < 250.f)
			bIsGonnaPush = true;
		else
			bIsGonnaPush = false;	
	}
}

void UAB_RobotArms_AnimInstance::PushSoundCube_Implementation(const UPrimitiveComponent* _pComponent)
{
	if (auto OwningPawn = Cast<AAB_Pawn>(TryGetPawnOwner()))
	{
		SoundCubeTransform = _pComponent->GetComponentLocation();
	}
}
