// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_Pawn.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAB_Pawn::AAB_Pawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	pMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	pCapsule->SetCapsuleHalfHeight(100.f);
	pCapsule->SetCapsuleRadius(50.f);
	pCapsule->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	RootComponent = pCapsule;

	pMesh->SetupAttachment(pCapsule);
	pCamera->SetupAttachment(pCapsule);

	pMesh->SetRelativeLocationAndRotation(FVector(50.f, 0.f, 0.f), FRotator(0.f, -90.f, 0.f));
	pCamera->SetRelativeLocationAndRotation(FVector(50.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));

	// static ConstructorHelpers::FObjectFinder<UMeshComponent> AB_SUBMARINE(TEXT(""));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AB_SUBMARINE(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/_03_BuildingSoundBlock/Meshes/AB_DefaultSkeletalMesh.AB_DefaultSkeletalMesh'"));
	if (AB_SUBMARINE.Succeeded())
		pMesh->SetSkeletalMesh(AB_SUBMARINE.Object);
}

// Called when the game starts or when spawned
void AAB_Pawn::BeginPlay()
{
	Super::BeginPlay();
}

void AAB_Pawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AAB_Pawn::PossessedBy(AController* _NewController)
{
	Super::PossessedBy(_NewController);
}

void AAB_Pawn::PlaneMove_Forward(float _Value)
{
	if (!FMath::IsNearlyEqual(_Value, 0.f))
	{
		const float TargetSpeed = _Value * Speed_Max;
		CurrentSpeed_Forward = FMath::FInterpTo(CurrentSpeed_Forward, TargetSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

		const FVector NewLocation = GetActorLocation() + GetActorForwardVector() * CurrentSpeed_Forward * GetWorld()->
			GetDeltaSeconds();
		SetActorLocation(NewLocation);
	}
	else
		CurrentSpeed_Forward = FMath::FInterpTo(CurrentSpeed_Forward, 0.0f, GetWorld()->GetDeltaSeconds(), 2.f);
}

void AAB_Pawn::PlaneMove_Right(float _Value)
{
	if (!FMath::IsNearlyEqual(_Value, 0.f))
	{
		const float TargetSpeed = _Value * Speed_Max;
		CurrentSpeed_Right = FMath::FInterpTo(CurrentSpeed_Right, TargetSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

		const FVector NewLocation = GetActorLocation() + GetActorRightVector() * CurrentSpeed_Right * GetWorld()->
			GetDeltaSeconds();
		SetActorLocation(NewLocation);
	}
	else
		CurrentSpeed_Right = FMath::FInterpTo(CurrentSpeed_Right, 0.0f, GetWorld()->GetDeltaSeconds(), 2.f);
}

void AAB_Pawn::ProcessMouseInputY(float _Value) { ProcessPitch(_Value); }
void AAB_Pawn::ProcessMouseInputZ(float _Value) { ProcessYaw(_Value); }

void AAB_Pawn::ProcessPitch(float _Value)
{
	const float TargetSpeedPitch = _Value * RateMultiplierPitch;
	CurrentSpeed_Pitch = FMath::FInterpTo(CurrentSpeed_Pitch, TargetSpeedPitch, GetWorld()->GetDeltaSeconds(), 2.f);
}

void AAB_Pawn::ProcessYaw(float _Value)
{
	const float TargetSpeedYaw = _Value * RateMultiplierYaw;
	CurrentSpeed_Yaw = FMath::FInterpTo(CurrentSpeed_Yaw, TargetSpeedYaw, GetWorld()->GetDeltaSeconds(), 2.f);
}

// Called every frame
void AAB_Pawn::Tick(float _DeltaTime)
{
	const float currentRollAngle = GetActorRotation().Roll;
	const float targetSpeedRoll = -currentRollAngle * RateMultiplierRoll; // RateMultiplierRoll은 설정에 따라 조절 가능한 보간 속도입니다.
	CurrentSpeed_Roll = FMath::FInterpTo(CurrentSpeed_Roll, targetSpeedRoll, _DeltaTime, 2.f);
	const FRotator newDeltaRotation(0.f, 0.f, CurrentSpeed_Roll * _DeltaTime);
	AddActorLocalRotation(newDeltaRotation);
	
	FRotator DeltaRotation(0.f, 0.f, 0.f);
	DeltaRotation.Pitch = CurrentSpeed_Roll * _DeltaTime;
	DeltaRotation.Pitch = CurrentSpeed_Pitch * _DeltaTime;
	DeltaRotation.Yaw = CurrentSpeed_Yaw * _DeltaTime;
	AddActorLocalRotation(DeltaRotation);

	Super::Tick(_DeltaTime);
}

// Called to bind functionality to input
void AAB_Pawn::SetupPlayerInputComponent(UInputComponent* _pPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_pPlayerInputComponent);

	_pPlayerInputComponent->BindAxis(TEXT("BackNForth"), this, &AAB_Pawn::PlaneMove_Forward);
	_pPlayerInputComponent->BindAxis(TEXT("LeftNRight"), this, &AAB_Pawn::PlaneMove_Right);
	
	_pPlayerInputComponent->BindAxis(TEXT("Turn"), this, &AAB_Pawn::ProcessMouseInputZ);
	_pPlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AAB_Pawn::ProcessMouseInputY);

	_pPlayerInputComponent->BindAction(TEXT("PreHoldCube"), EInputEvent::IE_Pressed, this, &AAB_Pawn::PrePushSoundCube);
	_pPlayerInputComponent->BindAction(TEXT("HoldCube"), EInputEvent::IE_Released, this, &AAB_Pawn::PushSoundCube);
	_pPlayerInputComponent->BindAction(TEXT("MusicStart"), EInputEvent::IE_Pressed, this, &AAB_Pawn::MusicStart);
}

void AAB_Pawn::PrePushSoundCube()
{
	bIsEKeyDown = true;
}

void AAB_Pawn::PushSoundCube()
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

TArray<FHitResult> AAB_Pawn::SweepInRange()
{
	Controller->GetPlayerViewPoint(OUT PlayerViewPtLoc, OUT PlayerViewPtRot);

	SweepStartPt = PlayerViewPtLoc + 100.f * PlayerViewPtRot.Vector();
	SweepEndPt = PlayerViewPtLoc + PlayerViewPtRot.Vector() * 500.f;

	//DrawDebugLine(GetWorld(), SweepStartPt, SweepEndPt, FColor::Red, true, 0.0f);

	const FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner());
	GetWorld()->SweepMultiByChannel(HitResults, SweepStartPt, SweepEndPt, FQuat::Identity, ECC_Visibility,
	                                FCollisionShape::MakeSphere(0.5f), TraceParams);

	return HitResults;
}

bool AAB_Pawn::IsGrounded(const UPrimitiveComponent* _pCubeComponent)
{
	_pCubeComponent->GetName().ParseIntoArray(CubeNames_Hit, TEXT(" "));

	if (CubeNames_Hit[2] != "0")
	{
		for (auto It = pAB_SoundCube->GetComponents().CreateConstIterator(); It; ++It)
		{
			if (const UStaticMeshComponent* pCube_Actor = Cast<UStaticMeshComponent>(*It))
			{
				pCube_Actor->GetName().ParseIntoArray(CubeNames_Actor, TEXT(" "));
				if (CubeNames_Hit[0] == CubeNames_Actor[0] && CubeNames_Hit[1] == CubeNames_Actor[1])
				{
					if (FCString::Atoi(*CubeNames_Hit[2]) > FCString::Atoi(*CubeNames_Actor[2]) && Cast<
						UStaticMeshComponent>(*It)->IsVisible() == false)
					{
						AB2LOG(Warning, TEXT("'Cause (%s) is empty, (%s) IS NOT GROUNDED!!!"), *pCube_Actor->GetName(),
						       *_pCubeComponent->GetName());
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

void AAB_Pawn::MusicStart()
{
	
}

