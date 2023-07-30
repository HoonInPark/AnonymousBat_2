// Fill out your copyright notice in the Description page of Project Settings.

///
/// Todo: 스윕을 썼을 때 가장 밑에 있는 녀석부터 visible이 되도록 -> 원래는 정육면체였던 안보이는 메시 컴포넌트가 사용자가 뭘 들고 있느냐에 따라 형태를 달리 하여 visible이 되도록 -> 
/// 

#include "AB_Character.h"

// Sets default values
AAB_Character::AAB_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Reach = 500.0f;
	bIsEKeyDown = false;
}

// Called when the game starts or when spawned
void AAB_Character::BeginPlay()
{
	Super::BeginPlay();

}

void AAB_Character::ProcessKeyPitch(float _Rate)
{
}

void AAB_Character::ProcessKeyRoll(float _Rate)
{
}

void AAB_Character::ProcessMouseInputY(float _Value)
{
}

void AAB_Character::ProcessMouseInputX(float _Value)
{
}

void AAB_Character::ProcessRoll(float _Value)
{
}

void AAB_Character::ProcessPitch(float _Value)
{
}

// Called every frame
void AAB_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAB_Character::SetupPlayerInputComponent(UInputComponent* _pPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_pPlayerInputComponent);

	
	// _pPlayerInputComponent->BindAction("PreHoldCube", EInputEvent::IE_Pressed, this, &AAB_Character::PrePushSoundCube);
	_pPlayerInputComponent->BindAction("HoldCube", EInputEvent::IE_Released, this, &AAB_Character::PushSoundCube);
}

/// <summary>
/// E 키를 누르면 스윕이 켜지고, E키를 떼면 스윕이 꺼짐과 동시에 마지막으로 닿은 녀석들이 나타난다.
/// 스윕이 켜진 상태에서 닿은 녀석들은 머티리얼이 투명한 것으로 변경된다.
/// 그리고 더이상 닿지 않게 되면 원래 안보이는 상태로 변경된다.
/// </summary>
void AAB_Character::PrePushSoundCube()
{
	bIsEKeyDown = true;
}

void AAB_Character::PushSoundCube()
{
	bIsEKeyDown = false;

	HitResults = SweepInRange();
	if (!HitResults.IsEmpty())
	{
		pAB_SoundCube = Cast<AAB_SoundCube_2>(HitResults.GetData()->GetActor());
		AB2CHECK(nullptr != pAB_SoundCube);

		if (pAB_SoundCube)
		{
			for (auto& hitResult : HitResults)
			{
				if (IsGrounded(hitResult.GetComponent()))
				{
					hitResult.GetComponent()->SetVisibility(true);
					hitResult.GetComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
					AB2LOG(Warning, TEXT("Push Sweeping has hit: (%s)"), *(hitResult.GetComponent()->GetName()));
				}

				AB2LOG(Warning, TEXT("Push Sweeping has hit: (%s)"), *(hitResult.GetComponent()->GetName()));
			}
		}
	}
}

TArray<FHitResult> AAB_Character::SweepInRange()
{
	Controller->GetPlayerViewPoint(OUT PlayerViewPtLoc, OUT PlayerViewPtRot);

	SweepStartPt = PlayerViewPtLoc + 100.0f * PlayerViewPtRot.Vector();
	SweepEndPt = PlayerViewPtLoc + PlayerViewPtRot.Vector() * 500.0f;

	//DrawDebugLine(GetWorld(), SweepStartPt, SweepEndPt, FColor::Red, true, 0.0f);

	const FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner()); 
	GetWorld()->SweepMultiByChannel(HitResults, SweepStartPt, SweepEndPt, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(0.5f), TraceParams);

	return HitResults;
}

bool AAB_Character::IsGrounded(const UPrimitiveComponent* _pCubeComponent)
{
	_pCubeComponent->GetName().ParseIntoArray(CubeNames_Hit, TEXT(" "));

	if (CubeNames_Hit[2] != "0")
	{
		for (auto It = pAB_SoundCube->GetComponents().CreateConstIterator(); It; ++It) // 스태틱메시컴포넌트의 이름을 가져와야 함!
		{
			if (const UStaticMeshComponent* pCube_Actor = Cast<UStaticMeshComponent>(*It))
			{
				pCube_Actor->GetName().ParseIntoArray(CubeNames_Actor, TEXT(" "));
				if (CubeNames_Hit[0] == CubeNames_Actor[0] && CubeNames_Hit[1] == CubeNames_Actor[1])
				{
					if (FCString::Atoi(*CubeNames_Hit[2]) > FCString::Atoi(*CubeNames_Actor[2]) && Cast<UStaticMeshComponent>(*It)->IsVisible() == false)
					{
						AB2LOG(Warning, TEXT("'Cause (%s) is empty, (%s) IS NOT GROUNDED!!!"), *pCube_Actor->GetName(), *_pCubeComponent->GetName());
						return false;
					}
				}
			}
		}
	}
	else
		return true;
	return true;
}
