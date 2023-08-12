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
		"/Script/Engine.SkeletalMesh'/Game/_05_DancingCubes/Animations/Full/R/RobotArm_v08_R_Anim.RobotArm_v08_R_Anim'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AB_ROBOTARMS_L(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/_05_DancingCubes/Rigs/SkeletalMeshes/RobotArm_v08_L_Anim02_Reverse_New.RobotArm_v08_L_Anim02_Reverse_New'"));

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
	const float targetSpeedRoll = -currentRollAngle * RateMultiplierRoll; // RateMultiplierRoll�� ������ ���� ���� ������ ���� �ӵ�.
	CurrentSpeed_Roll = FMath::FInterpTo(CurrentSpeed_Roll, targetSpeedRoll, _DeltaTime, 2.f);

	FRotator DeltaRotation;
	DeltaRotation.Pitch = CurrentSpeed_Pitch * _DeltaTime; // ��/�Ʒ�!
	DeltaRotation.Yaw = CurrentSpeed_Yaw * _DeltaTime; // �� ��!
	DeltaRotation.Roll = CurrentSpeed_Roll * _DeltaTime;

	AddActorLocalRotation(DeltaRotation, true);
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if (bIsMouseButtonDown) // ���콺�� ���� �ִ� ���¿��� ť�긦 ��� ������...
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

			// ���⼭ �бⰡ �ʿ�. ���� �༮�� � Ŭ�������� �����ؾ�!
			if (AActor* pHitActor = Hit_pressed.GetData()->GetActor())
			{
				pClosestHitCube = ClosestHitResult.GetComponent();
				if (pHitActor->GetClass() == AAB_SoundCube_2::StaticClass() && pClosestHitCube->GetCollisionObjectType()
					!= ECC_WorldDynamic)
				{
					pAB_SoundCube = Cast<AAB_SoundCube_2>(pHitActor);
					if (IsGrounded(pClosestHitCube))
					{
						IAB_Pawn_To_SoundCube_Interface::Execute_SoundCubeVisualizer_MouseButtonDown(
							pAB_SoundCube, pClosestHitCube);
						IAB_Pawn_To_AnimInst_Interface::Execute_PrePushSoundCube(pAnimInstance, pClosestHitCube);
						AttachMeshWithDelay();
					}
				}
				else if (Cast<AAB_SoundCube_Prepared>(pHitActor) && pClosestHitCube->IsVisible())
				{
					AAB_SoundCube_Prepared* pAB_SoundCube_Prepared = Cast<AAB_SoundCube_Prepared>(pHitActor);
					IAB_Pawn_To_SoundCube_Interface::Execute_SoundCubeVisualizer_MouseButtonDown(
						pAB_SoundCube_Prepared, pClosestHitCube);
					IAB_Pawn_To_AnimInst_Interface::Execute_PrePushSoundCube(pAnimInstance, pClosestHitCube);
					AttachMeshWithDelay();
				}

				AB2LOG(Warning, TEXT("%s"), *pHitActor->GetClass()->GetName());
				AB2LOG(Warning, TEXT("%s"), *AAB_SoundCube_Prepared::StaticClass()->GetName());
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
			IAB_Pawn_To_SoundCube_Interface::Execute_SoundCubeVisualizer_MouseButtonUp(pAB_SoundCube, pClosestHitCube);
			IAB_Pawn_To_AnimInst_Interface::Execute_PushSoundCube(pAnimInstance, pClosestHitCube);
		}
	}
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

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
	SweepEndPt = PlayerViewPtLoc + PlayerViewPtRot.Vector() * 200.f;

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
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttachDelay, this, &AAB_Pawn::AttachMeshToSocket, 0.6f, false);
	}
}

void AAB_Pawn::AttachMeshToSocket()
{
	if (bShouldAttach)
	{
		// ���ϴ� SkeletalMesh�� ���ϴ� ���Ͽ� �����ϱ� ���� �ڵ� (����: pSkeletalMesh_R)
		// �� �κ��� �ڽ��� ���̷�Ż �޽� ������Ʈ�� ���� �̸����� �����ؾ� ��
		const FName SoundCubeHeldSocket(TEXT("Joint_3Socket"));

		if (pSkeletalMesh_R->DoesSocketExist(SoundCubeHeldSocket))
		{
			pSoundCubeHeld = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SOUNDCUBEHELD"));
			static ConstructorHelpers::FObjectFinder<USkeletalMesh> SoundCubeHeldFinder_0(
				TEXT(
					"/Script/Engine.SkeletalMesh'/Game/_05_DancingCubes/Meshes/SK_SoundCube_Held/SkeletalMesh/AB_Cube.AB_Cube_UCX_AB_Cube'"));
			static ConstructorHelpers::FObjectFinder<USkeletalMesh> SoundCubeHeldFinder_1(
				TEXT(
					"/Script/Engine.SkeletalMesh'/Game/_05_DancingCubes/Meshes/SK_SoundCube_Held/SkeletalMesh/Actor01_3_Actor01_3.Actor01_3_Actor01_3'"));
			static ConstructorHelpers::FObjectFinder<USkeletalMesh> SoundCubeHeldFinder_2(
				TEXT(
					"/Script/Engine.SkeletalMesh'/Game/_05_DancingCubes/Meshes/SK_SoundCube_Held/SkeletalMesh/Actor02_3_Actor02_3.Actor02_3_Actor02_3'"));
			static ConstructorHelpers::FObjectFinder<USkeletalMesh> SoundCubeHeldFinder_3(
				TEXT(
					"/Script/Engine.SkeletalMesh'/Game/_05_DancingCubes/Meshes/SK_SoundCube_Held/SkeletalMesh/Actor03_3_Actor03_3.Actor03_3_Actor03_3'"));

			if (SoundCubeHeldFinder_0.Succeeded() && SoundCubeHeldFinder_1.Succeeded() && SoundCubeHeldFinder_2.Succeeded() && SoundCubeHeldFinder_3.Succeeded())
			{
				pSoundCubeHeld->SetSkeletalMesh(SoundCubeHeldFinder_0.Object);
				pSoundCubeHeld->AttachToComponent(pSkeletalMesh_R, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SoundCubeHeldSocket);
				bShouldAttach = false;
			}
		}
	}
}

void AAB_Pawn::MusicStart_Implementation()
{
	IAB_Pawn_To_SoundCube_Interface::Execute_MusicStart(
		UGameplayStatics::GetActorOfClass(GetWorld(), AAB_SoundCube_2::StaticClass()));
}



///
///	<�����ؾ� �ϴ� ��ɵ�>
///�����̽��ٸ� ������ ��� ����ť���� ũ�Ⱑ Ŀ���鼭 �� ť�꿡 �Ҵ�� ������ ���յǾ� �ϳ��� ������ �ǵ���
///
///<�κ귯�� �ִ� ť�꿡 ���� ����� �Ҵ��ϰ� �̰��� �������� �� �ϳ��� �������� �ǵ��� �Ϸ���>
///1. ť�긦 ���� ���¿����� ���� ����� ����ž� ��. => ���� ������ ���콺�� �������� �̺�Ʈ��
///�� �� ������ �� ť�꿡�� ������ �귯������ ���� �ƴϰ�, ��Ÿ�� ���� �� ť���� ��ȣ�� ���� ����� ������ �迭�� ������ �ȴ�.
///ť�긦 ������ �� ���� ����� �ε��Ͽ� ����Ѵ�.
///ť�� ��ü�� ������ �Ҵ��ϴ°� �ƴ϶�, ��� ������ ���� ����� �ҷ����� ������.
///2. ť�긦 �װ� �����̽��ٸ� ������ �� ��� ����� ����ž� ��.
///
///<ť�꿡 ���� ���� �Ҵ� ����>
/// bass 10
/// drum 5
/// elec 3
/// guitar, aucoustic 4
/// piano 5
///
/// AAB_Pawn Ŭ�������� ���� ��� �ִ� ť�꿡 �ش�� ������ ������
/// AAB_SoundCube_2 Ŭ�������� ���Ҵ� �༮���� ������ �ִ��� �������� ���´�.
/// ���� �߰�... ���� �����ϸ� ���� �����...
