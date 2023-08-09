// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_Pawn.h"
#include "AB_RobotArms_AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AAB_Pawn::AAB_Pawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsMouseButtonDown = false;

	pBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	pSkeletalMesh_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKELETAL_R"));
	pSkeletalMesh_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKELETAL_L"));
	pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = pBodyMesh;

	pCamera->SetupAttachment(pBodyMesh);
	pSkeletalMesh_R->SetupAttachment(pBodyMesh);
	pSkeletalMesh_L->SetupAttachment(pBodyMesh);

	pBodyMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	pSkeletalMesh_R->SetRelativeLocationAndRotation(FVector(0.f, 50.f, -35.f), FRotator(0.f, -90.f, 0.f));
	pSkeletalMesh_L->SetRelativeLocationAndRotation(FVector(0.f, -50.f, -35.f), FRotator(0.f, -90.f, 0.f));
	pCamera->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AB_SUBMARINE(TEXT(
		"/Script/Engine.StaticMesh'/Game/_04_RobotArms/Meshes/submarine_complete3.submarine_complete3'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AB_ROBOTARMS_R(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/_04_RobotArms/Rigs/RobotArm_v07_R.RobotArm_v07_R'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AB_ROBOTARMS_L(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/_04_RobotArms/Rigs/RobotArm_v07_L.RobotArm_v07_L'"));

	if (AB_SUBMARINE.Succeeded() && AB_ROBOTARMS_R.Succeeded() && AB_ROBOTARMS_L.Succeeded())
	{
		pBodyMesh->SetStaticMesh(AB_SUBMARINE.Object);
		pBodyMesh->SetCollisionProfileName(TEXT("ABPawn"));

		pSkeletalMesh_R->SetSkeletalMesh(AB_ROBOTARMS_R.Object);
		pSkeletalMesh_R->SetWorldScale3D(FVector(4.f, 4.f, 4.f));
		pSkeletalMesh_L->SetSkeletalMesh(AB_ROBOTARMS_L.Object);
		pSkeletalMesh_L->SetWorldScale3D(FVector(4.f, 4.f, 4.f));
	}
}

void AAB_Pawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UAnimInstance* CurrentAnimInstance = pSkeletalMesh_R->GetAnimInstance();

	if (!CurrentAnimInstance)
	{
		pAnimInstance = NewObject<UAB_RobotArms_AnimInstance>(pSkeletalMesh_R,
		                                                      UAB_RobotArms_AnimInstance::StaticClass());
		pSkeletalMesh_R->SetAnimInstanceClass(pAnimInstance->GetClass());
	}
	else
		pAnimInstance = Cast<UAB_RobotArms_AnimInstance>(CurrentAnimInstance);
}

// Called when the game starts or when spawned
void AAB_Pawn::BeginPlay() { Super::BeginPlay(); }
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

	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentSpeed_Pitch * _DeltaTime; // 위/아래!
	DeltaRotation.Yaw = CurrentSpeed_Yaw * _DeltaTime; // 양 옆!
	DeltaRotation.Roll = CurrentSpeed_Roll * _DeltaTime;

	AddActorLocalRotation(DeltaRotation, true);

	if (bIsMouseButtonDown)
	{
		Hit_pressed = SweepInRange();

		if (!Hit_pressed.IsEmpty())
		{
			float MinDistance = FLT_MAX;

			for (const auto& pHitResult : Hit_pressed)
			{
				const float Distance = FVector::Dist(SweepStartPt, pHitResult.ImpactPoint);
				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					ClosestHitResult = pHitResult;
				}
			}

			pAB_SoundCube = Cast<AAB_SoundCube_2>(Hit_pressed.GetData()->GetActor());
			pClosestHitCube = ClosestHitResult.GetComponent();
			if (pAB_SoundCube && IsGrounded(pClosestHitCube))
			{
				IAB_Pawn_To_SoundCube_Interface::Execute_SoundCubeVisualizer_MouseButtonDown(
					pAB_SoundCube, pClosestHitCube);
				// IAB_Pawn_To_AnimInst_Interface::Execute_PrePushSoundCube(pAnimInstance, pClosestHitCube);
			}
		}
	}
}

// Called to bind functionality to input
void AAB_Pawn::SetupPlayerInputComponent(UInputComponent* _pPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_pPlayerInputComponent);

	_pPlayerInputComponent->BindAxis(TEXT("BackNForth"), this, &AAB_Pawn::PlaneMove_Forward);
	_pPlayerInputComponent->BindAxis(TEXT("LeftNRight"), this, &AAB_Pawn::PlaneMove_Right);

	_pPlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AAB_Pawn::ProcessPitch);
	_pPlayerInputComponent->BindAxis(TEXT("Turn"), this, &AAB_Pawn::ProcessYaw);

	_pPlayerInputComponent->BindAction(TEXT("PreHoldCube"), EInputEvent::IE_Pressed, this,
	                                   &AAB_Pawn::CallPrePushSoundCube);
	_pPlayerInputComponent->BindAction(TEXT("HoldCube"), EInputEvent::IE_Released, this,
	                                   &AAB_Pawn::CallPushSoundCube);
	_pPlayerInputComponent->BindAction(TEXT("MusicStart"), EInputEvent::IE_Pressed, this,
	                                   &AAB_Pawn::MusicStart_Implementation);
}

void AAB_Pawn::CallPrePushSoundCube() { PrePushSoundCube_Implementation(nullptr); }
void AAB_Pawn::CallPushSoundCube() { PushSoundCube_Implementation(nullptr); }

void AAB_Pawn::PrePushSoundCube_Implementation(const UPrimitiveComponent* _pComponent) { bIsMouseButtonDown = true; }

void AAB_Pawn::PushSoundCube_Implementation(const UPrimitiveComponent* _pComponent)
{
	bIsMouseButtonDown = false;

	Hit_released = SweepInRange();
	if (!Hit_released.IsEmpty())
	{
		float MinDistance = FLT_MAX;

		for (const auto& pHitResult : Hit_released)
		{
			const float Distance = FVector::Dist(SweepStartPt, pHitResult.ImpactPoint);
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				ClosestHitResult = pHitResult;
			}
		}

		pAB_SoundCube = Cast<AAB_SoundCube_2>(Hit_released.GetData()->GetActor());
		pClosestHitCube = ClosestHitResult.GetComponent();
		if (pAB_SoundCube && IsGrounded(pClosestHitCube))
		{
			IAB_Pawn_To_SoundCube_Interface::Execute_SoundCubeVisualizer_MouseButtonUp(pAB_SoundCube, pClosestHitCube);
			IAB_Pawn_To_AnimInst_Interface::Execute_PushSoundCube(pAnimInstance, pClosestHitCube);
		}
	}
}

void AAB_Pawn::SoundCubeVisualizer_MouseButtonDown_Implementation(UPrimitiveComponent* _ClosestHit)
{
}

void AAB_Pawn::SoundCubeVisualizer_MouseButtonUp_Implementation(UPrimitiveComponent* _ClosestHit)
{
}

TArray<FHitResult> AAB_Pawn::SweepInRange()
{
	Controller->GetPlayerViewPoint(OUT PlayerViewPtLoc, OUT PlayerViewPtRot);

	SweepStartPt = PlayerViewPtLoc + 100.f * PlayerViewPtRot.Vector();
	SweepEndPt = PlayerViewPtLoc + PlayerViewPtRot.Vector() * 250.f;

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
						return false;
				}
			}
		}
	}
	else
		return true;
	return true;
}

void AAB_Pawn::MusicStart_Implementation()
{
	IAB_Pawn_To_SoundCube_Interface::Execute_MusicStart(
		UGameplayStatics::GetActorOfClass(GetWorld(), AAB_SoundCube_2::StaticClass()));
}


///
///	<구현해야 하는 기능들>
///이미 큐브가 놓여져 있으면 거기에 스윕이 닿았을때 사라지도록. 큐브를 집는 것이다.
///스페이스바를 누르면 모든 사운드큐브의 크기가 커지면서 각 큐브에 할당된 음악이 종합되어 하나의 선율이 되도록
///
///<널브러져 있는 큐브에 사운드 모듈을 할당하고 이것이 합쳐졌을 때 하나의 음악으로 되도록 하려면>
///1. 큐브를 집은 상태에서는 사운드 모듈이 재생돼야 함. => 현재 구현된 마우스가 눌릴때의 이벤트는
///큐브를 이미 집고 있다는 가정 하에 만듦.
///
///[바인딩된 지점에서 bIsMouseButtonDown을 true로 만듦->
///Tick으로 간 다음 큐브 클래스의 인터페이스 호출->
///현재 Sweep이 되는 것 이외의 큐브를 SetVisibile(false)로 설정]
///
///
///큐브를 집으면 그 큐브가 사라져야 함
///집은 상태가 되면 pawn의 SkeletalMesh에 큐브 컴포넌트를 attach한다. => 인터페이스!
///실제로 그 큐브에서 음악이 흘러나오는 것은 아니고, 런타임 동안 그 큐브의 번호와 사운드 모듈이 대응된 배열이 있으면 된다.
///큐브를 집었을 때 사운드 모듈을 로드하여 재생한다.
///2. 큐브를 쌓고 스페이스바를 누르면 때 모든 모듈이 재생돼야 함.
///
///
