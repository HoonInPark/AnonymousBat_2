// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_Pawn.h"
#include "AB_RobotArms_AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AAB_Pawn::AAB_Pawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttachDelay = 0.f;
	bShouldAttach = false;
	bIsMouseButtonDown = false;
	CurrentAudioComponent = nullptr;

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
		"/Script/Engine.StaticMesh'/Game/_05_DancingCubes/Meshes/submarine_complete3.submarine_complete3'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AB_ROBOTARMS_R(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/_05_DancingCubes/Animations/R/SkeletalMeshes/RobotArm_v08_R_Stretch.RobotArm_v08_R_Stretch'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AB_ROBOTARMS_L(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/_05_DancingCubes/Animations/L/SkeletalMeshes/RobotArm_v08_L_Stretch.RobotArm_v08_L_Stretch'"));

	if (AB_SUBMARINE.Succeeded() && AB_ROBOTARMS_R.Succeeded() && AB_ROBOTARMS_L.Succeeded())
	{
		pBodyMesh->SetStaticMesh(AB_SUBMARINE.Object);
		pBodyMesh->SetCollisionProfileName(TEXT("ABPawn"));

		pSkeletalMesh_R->SetSkeletalMesh(AB_ROBOTARMS_R.Object);
		pSkeletalMesh_R->SetWorldScale3D(FVector(4.f, 4.f, 4.f));
		pSkeletalMesh_L->SetSkeletalMesh(AB_ROBOTARMS_L.Object);
		pSkeletalMesh_L->SetWorldScale3D(FVector(4.f, 4.f, 4.f));
	}

	SoundCubeHeldSocket = TEXT("Joint_3Socket");

	if (pSkeletalMesh_R->DoesSocketExist(SoundCubeHeldSocket))
	{
		pSoundCubeHeld = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SOUNDCUBEHELD"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SoundCubeHeldFinder_0(
			TEXT(
				"/Game/_05_DancingCubes/Meshes/SK_SoundCube_Held/Actor01.Actor01_Actor01"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SoundCubeHeldFinder_1(
			TEXT(
				"/Game/_05_DancingCubes/Meshes/SK_SoundCube_Held/Actor02.Actor02_Actor02"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SoundCubeHeldFinder_2(
			TEXT(
				"/Game/_05_DancingCubes/Meshes/SK_SoundCube_Held/Actor03.Actor03_Actor03"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SoundCubeHeldFinder_3(
			TEXT(
				"/Game/_05_DancingCubes/Meshes/SK_SoundCube_Held/Actor04.Actor04_Actor04"));
		if (SoundCubeHeldFinder_0.Succeeded() && SoundCubeHeldFinder_1.Succeeded() && SoundCubeHeldFinder_2.Succeeded()
			&& SoundCubeHeldFinder_3.Succeeded())
		{
			pCubeMeshesHeld.Add(SoundCubeHeldFinder_0.Object);
			pCubeMeshesHeld.Add(SoundCubeHeldFinder_1.Object);
			pCubeMeshesHeld.Add(SoundCubeHeldFinder_2.Object);
			pCubeMeshesHeld.Add(SoundCubeHeldFinder_3.Object);
		}
	}

	SoundPaths_Bass = {
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Bass/436506__red19official__bd_knocky_c.436506__red19official__bd_knocky_c'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Bass/463930__alienxxx__mean_bass_loop_130bpm_dminfinal.463930__alienxxx__mean_bass_loop_130bpm_dminfinal'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Bass/464443__prime45__bass-sound.464443__prime45__bass-sound'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Bass/500259__matrixxx__european-fashion-background-bass-rhythm-04-rep.500259__matrixxx__european-fashion-background-bass-rhythm-04-rep'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Bass/500260__matrixxx__european-fashion-background-bass-rhythm-32-rep.500260__matrixxx__european-fashion-background-bass-rhythm-32-rep'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Bass/529808__logicogonist__bass-loop-and-drums-130-bpm.529808__logicogonist__bass-loop-and-drums-130-bpm'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Bass/621799__tkky__electric-bass-guitar-loop.621799__tkky__electric-bass-guitar-loop'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Bass/623189__deadrobotmusic__web-crawler-bass.623189__deadrobotmusic__web-crawler-bass'"
	};
	SoundPaths_Drum = {
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Drum/381353__waveplaysfx__drumloop-120-bpm-edm-drum-loop-022.381353__waveplaysfx__drumloop-120-bpm-edm-drum-loop-022'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Drum/439688__inspectorj__ocean-drum-rolling-felt-a.439688__inspectorj__ocean-drum-rolling-felt-a'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Drum/610823__paraplex__fresh-drum-loop.610823__paraplex__fresh-drum-loop'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Drum/629134__holizna__76-bpm-lofi-drum-loop.629134__holizna__76-bpm-lofi-drum-loop'"
	};
	SoundPaths_Guitar = {
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Guitar/370934__karolist__guitar-solo.370934__karolist__guitar-solo'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Guitar/533847__tosha73__distortion-guitar-power-chord-e.533847__tosha73__distortion-guitar-power-chord-e'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Guitar/629150__holizna__lofi-guitar-loop-em-119-bpm3.629150__holizna__lofi-guitar-loop-em-119-bpm3'"
	};
	SoundPaths_Piano = {
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Piano/645979__timouse__piano-composition-1.645979__timouse__piano-composition-1'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Piano/648259__timouse__piano-composition-3.648259__timouse__piano-composition-3'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Piano/648438__timouse__piano-loop-22.648438__timouse__piano-loop-22'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Piano/651877__timouse__piano-loop-25.651877__timouse__piano-loop-25'",
		"/Script/Engine.SoundWave'/Game/_05_DancingCubes/Sources/Sounds/Piano/680316__turkis__mallets-on-piano-114bpm.680316__turkis__mallets-on-piano-114bpm'"
	};

	ArrOfArr = {&SoundPaths_Bass, &SoundPaths_Drum, &SoundPaths_Guitar, &SoundPaths_Piano};
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

	FRotator DeltaRotation;
	DeltaRotation.Pitch = CurrentSpeed_Pitch * _DeltaTime; // 위/아래!
	DeltaRotation.Yaw = CurrentSpeed_Yaw * _DeltaTime; // 양 옆!
	DeltaRotation.Roll = CurrentSpeed_Roll * _DeltaTime;

	AddActorLocalRotation(DeltaRotation, true);
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if (bIsMouseButtonDown) // 마우스가 눌려 있는 상태에서 큐브를 들고 있으면...
	{
		Hit_pressed = SweepInRange();

		if (!Hit_pressed.IsEmpty()) // 만약 스윕에 닿은 게 비어있지 않다면...
		{
			float MinDistance = FLT_MAX;

			for (const auto& pHitResult : Hit_pressed)
			{
				const float Distance = FVector::Dist(SweepStartPt, pHitResult.ImpactPoint);
				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					ClosestHitResult = pHitResult; // 그 중 가장 가까운 녀석을 구하고...
				}
			}

			// 닿은 녀석이 어떤 클래스인지 구분하고자...
			if (AActor* pHitActor = Hit_pressed.GetData()->GetActor())
			{
				pAB_SoundCube = Cast<AAB_SoundCube_2>(pHitActor);
				pClosestHitCube = ClosestHitResult.GetComponent(); // 닿은 녀석의 컴포넌트를 구하고...
				if (pAB_SoundCube && pClosestHitCube->GetCollisionObjectType() != ECC_WorldStatic)
				// 클래스의 종류를 판별 & 가장 가까이 닿은 녀석이 Visible이 꺼져있는지 판별
				{
					if (IsGrounded(pClosestHitCube))
					{
						IAB_Pawn_To_SoundCube_Interface::Execute_SoundCubeVisualizer_MouseButtonDown(
							pAB_SoundCube, pClosestHitCube, this);
						IAB_Pawn_To_AnimInst_Interface::Execute_PrePushSoundCube(pAnimInstance, pClosestHitCube);
						AttachMeshWithDelay();
					}
				}
				else if (Cast<AAB_SoundCube_Prepared>(pHitActor) && pClosestHitCube->IsVisible())
				{
					pAB_SoundCube_Prepared = Cast<AAB_SoundCube_Prepared>(pHitActor);
					IAB_Pawn_To_SoundCube_Interface::Execute_SoundCubeVisualizer_MouseButtonDown(
						pAB_SoundCube_Prepared, pClosestHitCube, this);
					IAB_Pawn_To_AnimInst_Interface::Execute_PrePushSoundCube(pAnimInstance, pClosestHitCube);
					AttachMeshWithDelay();
				}
			}
		}
	}
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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
			IAB_Pawn_To_SoundCube_Interface::Execute_SoundCubeVisualizer_MouseButtonUp(
				pAB_SoundCube, pClosestHitCube, this);
			IAB_Pawn_To_AnimInst_Interface::Execute_PushSoundCube(pAnimInstance, pClosestHitCube);

			if (pSoundCubeHeld)
			{
				pSoundCubeHeld->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
				pSoundCubeHeld->SetSkeletalMesh(nullptr); 
			}
		}
	}
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void AAB_Pawn::SoundCubeVisualizer_MouseButtonDown_Implementation(UPrimitiveComponent* _ClosestHit, AAB_Pawn* _pCaller)
{
}

void AAB_Pawn::SoundCubeVisualizer_MouseButtonUp_Implementation(UPrimitiveComponent* _ClosestHit, AAB_Pawn* _pCaller)
{
	StopMusic();
}

TArray<FHitResult> AAB_Pawn::SweepInRange()
{
	Controller->GetPlayerViewPoint(OUT PlayerViewPtLoc, OUT PlayerViewPtRot);

	SweepStartPt = PlayerViewPtLoc + 100.f * PlayerViewPtRot.Vector();
	SweepEndPt = PlayerViewPtLoc + PlayerViewPtRot.Vector() * 200.f;

	// DrawDebugLine(GetWorld(), SweepStartPt, SweepEndPt, FColor::Red, true, 0.0f);

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
						UStaticMeshComponent>(*It)->GetCollisionObjectType() != ECC_WorldStatic)
						return false;
				}
			}
		}
	}
	else
		return true;
	return true;
}

void AAB_Pawn::AttachMeshWithDelay()
{
	if (!bShouldAttach)
	{
		bShouldAttach = true;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttachDelay, this, &AAB_Pawn::AttachMeshToSocket, 1.f,
		                                       false);
	}
}

void AAB_Pawn::PlayMusic(USoundWave* SoundWave)
{
	StopMusic(); // Stop any music currently playing

	CurrentAudioComponent = UGameplayStatics::SpawnSound2D(this, SoundWave);
	if (CurrentAudioComponent)
	{
		CurrentAudioComponent->OnAudioFinished.AddDynamic(this, &AAB_Pawn::OnAudioFinished);
	}
}

void AAB_Pawn::StopMusic()
{
	if (CurrentAudioComponent)
	{
		CurrentAudioComponent->Stop();
		CurrentAudioComponent = nullptr;
	}
}

void AAB_Pawn::OnAudioFinished()
{
	// This function can be used to handle any logic that should happen when a sound finishes playing
	// ...
	CurrentAudioComponent = nullptr;
}

void AAB_Pawn::AttachMeshToSocket()
{
	pSoundCubeHeld->SetSkeletalMesh(pCubeMeshesHeld[pAB_SoundCube_Prepared->StaticMeshNum]);
	pSoundCubeHeld->AttachToComponent(pSkeletalMesh_R, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                                  SoundCubeHeldSocket);

	PlayRandomSoundFromCategory(pAB_SoundCube_Prepared->StaticMeshNum);
	bShouldAttach = false;
}

void AAB_Pawn::MusicStart_Implementation()
{
	IAB_Pawn_To_SoundCube_Interface::Execute_MusicStart(
		UGameplayStatics::GetActorOfClass(GetWorld(), AAB_SoundCube_2::StaticClass()));
}

void AAB_Pawn::PlayRandomSoundFromCategory(int32 _CategoryNum)
{
	const int32 RandomIndex = FMath::RandRange(0, ArrOfArr[_CategoryNum]->Num() - 1);
	const FString& RandomSoundPath = ArrOfArr[_CategoryNum]->Pop(true);

	USoundWave* SoundWave = LoadObject<USoundWave>(nullptr, *RandomSoundPath);

	if (!SoundWave)
		return;

	PlayMusic(SoundWave);
}
