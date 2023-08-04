// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_Pawn.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AAB_Pawn::AAB_Pawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsEKeyDown = false;

	pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = pMesh;

	pCamera->SetupAttachment(pMesh);

	pMesh->SetRelativeLocationAndRotation(FVector(50.f, 0.f, 0.f), FRotator(0.f, -90.f, 0.f));
	pCamera->SetRelativeLocationAndRotation(FVector(50.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));

	// static ConstructorHelpers::FObjectFinder<UMeshComponent> AB_SUBMARINE(TEXT(""));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> AB_SUBMARINE(TEXT(
		"/Script/Engine.StaticMesh'/Game/_03_BuildingSoundBlock/Meshes/AB_SM_Shape_SoundBlock.AB_SM_Shape_SoundBlock'"));
	if (AB_SUBMARINE.Succeeded())
	{
		pMesh->SetStaticMesh(AB_SUBMARINE.Object);
		pMesh->SetCollisionProfileName(TEXT("ABPawn"));
	}
}

// Called when the game starts or when spawned
void AAB_Pawn::BeginPlay() { Super::BeginPlay(); }
void AAB_Pawn::PostInitializeComponents() {	Super::PostInitializeComponents(); }
void AAB_Pawn::PossessedBy(AController* _NewController) { Super::PossessedBy(_NewController); }

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

void AAB_Pawn::ProcessPitch(float _Value)
{
	const float TargetSpeedPitch = _Value * RateMultiplierPitch;
	CurrentSpeed_Pitch = FMath::FInterpTo(CurrentSpeed_Pitch, TargetSpeedPitch, GetWorld()->GetDeltaSeconds(), 2.f);
	CurrentSpeed_Pitch = FMath::Clamp(CurrentSpeed_Pitch, -300, 300);
}

void AAB_Pawn::ProcessYaw(float _Value)
{
	const float TargetSpeedYaw = _Value * RateMultiplierYaw;
	CurrentSpeed_Yaw = FMath::FInterpTo(CurrentSpeed_Yaw, TargetSpeedYaw, GetWorld()->GetDeltaSeconds(), 2.f);
	CurrentSpeed_Yaw = FMath::Clamp(CurrentSpeed_Yaw, -300, 300);
}

// Called every frame
void AAB_Pawn::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	const float currentRollAngle = GetActorRotation().Roll;
	const float targetSpeedRoll = -currentRollAngle * RateMultiplierRoll; // RateMultiplierRoll은 설정에 따라 조절 가능한 보간 속도.
	CurrentSpeed_Roll = FMath::FInterpTo(CurrentSpeed_Roll, targetSpeedRoll, _DeltaTime, 2.f);

	FRotator DeltaRotation(0,0,0);
	DeltaRotation.Pitch = CurrentSpeed_Pitch * _DeltaTime; // 위/아래!
	DeltaRotation.Yaw = CurrentSpeed_Yaw * _DeltaTime; // 양 옆!
	DeltaRotation.Roll = CurrentSpeed_Roll * _DeltaTime;

	AddActorLocalRotation(DeltaRotation,true);
}

// Called to bind functionality to input
void AAB_Pawn::SetupPlayerInputComponent(UInputComponent* _pPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_pPlayerInputComponent);

	_pPlayerInputComponent->BindAxis(TEXT("BackNForth"), this, &AAB_Pawn::PlaneMove_Forward);
	_pPlayerInputComponent->BindAxis(TEXT("LeftNRight"), this, &AAB_Pawn::PlaneMove_Right);

	_pPlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AAB_Pawn::ProcessPitch);
	_pPlayerInputComponent->BindAxis(TEXT("Turn"), this, &AAB_Pawn::ProcessYaw);

	_pPlayerInputComponent->BindAction(TEXT("PreHoldCube"), EInputEvent::IE_Pressed, this, &AAB_Pawn::PrePushSoundCube);
	_pPlayerInputComponent->BindAction(TEXT("HoldCube"), EInputEvent::IE_Released, this, &AAB_Pawn::PushSoundCube);
	_pPlayerInputComponent->BindAction(TEXT("MusicStart"), EInputEvent::IE_Pressed, this, &AAB_Pawn::MusicStart);
}
void AAB_Pawn::PrePushSoundCube()
{
	bIsEKeyDown = true;
    
	Hit_pressed = SweepInRange();
	if (!Hit_pressed.IsEmpty())
	{
		FHitResult ClosestHitResult;
		float MinDistance = FLT_MAX;

		for (const auto& hitResult : Hit_pressed)
		{
			float Distance = FVector::Dist(SweepStartPt, hitResult.ImpactPoint);
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestHitResult = hitResult;
			}
		}

		pAB_SoundCube = Cast<AAB_SoundCube_2>(ClosestHitResult.GetActor());
		AB2CHECK(nullptr != pAB_SoundCube);

		if (pAB_SoundCube)
		{
			if (IsGrounded(ClosestHitResult.GetComponent()))
			{
				ClosestHitResult.GetComponent()->SetVisibility(true);
				ClosestHitResult.GetComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
			}
		}
	}
}

void AAB_Pawn::PushSoundCube()
{
	bIsEKeyDown = false;

	Hit_released = SweepInRange();
	if (!Hit_released.IsEmpty())
	{
		FHitResult ClosestHitResult;
		float MinDistance = FLT_MAX;

		for (const auto& hitResult : Hit_released)
		{
			float Distance = FVector::Dist(SweepStartPt, hitResult.ImpactPoint);
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestHitResult = hitResult;
			}
		}

		pAB_SoundCube = Cast<AAB_SoundCube_2>(ClosestHitResult.GetActor());
		AB2CHECK(nullptr != pAB_SoundCube);

		if (pAB_SoundCube)
		{
			if (IsGrounded(ClosestHitResult.GetComponent()))
			{
				ClosestHitResult.GetComponent()->SetVisibility(true);
				ClosestHitResult.GetComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
			}
		}
	}
}

TArray<FHitResult> AAB_Pawn::SweepInRange()
{
	Controller->GetPlayerViewPoint(OUT PlayerViewPtLoc, OUT PlayerViewPtRot);

	SweepStartPt = PlayerViewPtLoc + 100.f * PlayerViewPtRot.Vector();
	SweepEndPt = PlayerViewPtLoc + PlayerViewPtRot.Vector() * 500.f;

	DrawDebugLine(GetWorld(), SweepStartPt, SweepEndPt, FColor::Red, true, 0.0f);

	const FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner());
	GetWorld()->SweepMultiByChannel(Hit_released, SweepStartPt, SweepEndPt, FQuat::Identity, ECC_Visibility,
	                                FCollisionShape::MakeSphere(0.5f), TraceParams);

	return Hit_released;
}

bool AAB_Pawn::IsGrounded(const UPrimitiveComponent* _pCubeComponent)
{
	_pCubeComponent->GetName().ParseIntoArray(CubeNames_Hit, TEXT(" "));

	if (CubeNames_Hit.Num() < 3)
		return false;
	
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
	AB2LOG_S(Warning);
}

///
///	<구현해야 하는 기능들>
///이미 큐브가 놓여져 있으면 거기에 스윕이 닿았을때 사라지도록. 큐브를 집는 것이다.
///큐브가 없는데 큐브를 놓아야할 자리이거나 큐브가 있는 경우 스윕이 닿으면 (Visibility가 true인 상태에서)거기에 머티리얼의 색이 변하도록!
///스페이스바를 누르면 모든 사운드큐브의 크기가 커지면서 각 큐브에 할당된 음악이 종합되어 하나의 선율이 되도록
///테마를 선택하고 장면이 전환될 수 있도록.
///
