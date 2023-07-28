// Fill out your copyright notice in the Description page of Project Settings.


#include "AB_SoundCube_2.h"
#include "AB_Character.h"

// Sets default values
AAB_SoundCube_2::AAB_SoundCube_2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CubeSize = 50.0f;
	NumCubes = 10;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->SetMobility(EComponentMobility::Static);
	SetRootComponent(RootComponent);

	// Find and assign cube meshes
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder_Default(
		TEXT("/Script/Engine.StaticMesh'/Game/_03_BuildingSoundBlock/Meshes/AB_Cube.AB_Cube'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder_1(
		TEXT("/Script/Engine.StaticMesh'/Game/_03_BuildingSoundBlock/Meshes/Actor01_3.Actor01_3'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder_2(
		TEXT("/Script/Engine.StaticMesh'/Game/_03_BuildingSoundBlock/Meshes/Actor02_3.Actor02_3'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder_3(
		TEXT("/Script/Engine.StaticMesh'/Game/_03_BuildingSoundBlock/Meshes/Actor03_3.Actor03_3'"));

	if (CubeMeshFinder_Default.Succeeded() && CubeMeshFinder_1.Succeeded() && CubeMeshFinder_2.Succeeded() &&
		CubeMeshFinder_3.Succeeded())
	{
		pCubeMeshes.Add(CubeMeshFinder_Default.Object);
		pCubeMeshes.Add(CubeMeshFinder_1.Object);
		pCubeMeshes.Add(CubeMeshFinder_2.Object);
		pCubeMeshes.Add(CubeMeshFinder_3.Object);
	}

	// Create Cube Mesh Components
	if (NumCubes > 0 && pCubeMeshes.Num() > 0)
	{
		const float TotalLength = NumCubes * CubeSize;

		for (int32 k = 0; k < NumCubes; k++)
		{
			for (int32 j = 0; j < NumCubes; j++)
			{
				for (int32 i = 0; i < NumCubes; i++)
				{
					FString ComponentName = FString::Printf(TEXT("%d %d %d"), i, j, k);

					pCubeComponent = CreateDefaultSubobject<UStaticMeshComponent>(*ComponentName);
					pCubeComponent->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
					pCubeComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
					pCubeComponent->SetupAttachment(RootComponent);

					pCubeComponent->SetRelativeLocation(FVector(i * CubeSize * 0.4f, j * CubeSize * 0.4f,
					                                            k * CubeSize * 0.4f));
					// pCubeComponent->SetRelativeRotation(
					// 	FRotator(FMath::FRandRange(-30.0f, 30.0f), FMath::FRandRange(-30.0f, 30.0f), 0.0f));
					pCubeComponent->SetRelativeScale3D(FVector(CubeSize / 250.0f));
					pCubeComponent->SetMobility(EComponentMobility::Static);

					// 오버랩 이벤트 등록
					pCubeComponent->OnComponentBeginOverlap.AddDynamic(this, &AAB_SoundCube_2::OnOverlapBegin);
					pCubeComponent->OnComponentEndOverlap.AddDynamic(this, &AAB_SoundCube_2::OnOverlapEnd);

					// pCubeComponent->SetStaticMesh(pCubeMeshes[FMath::RandRange(1, pCubeMeshes.Num() - 1)]);
					pCubeComponent->SetStaticMesh(pCubeMeshes[0]);
					pCubeComponent->SetVisibility(false);
				}
			}
		}

		SetActorScale3D(FVector(TotalLength / 100.0f));
	}
}

// Called when the game starts or when spawned
void AAB_SoundCube_2::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAB_SoundCube_2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAB_SoundCube_2::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	// Todo: 먼저 메시가 무작위의 각도로 스폰되는 경우 겹침이벤트가 생기므로 이걸 무시하는 로직이 필요... 만약 이 무시하는 로직을 E키를 누르지도 않았는데 계속 실행한다면 너무 비효율적이다.
	// Todo: 그리고 캐릭터의 스윕에 쓰이는 콜리전만 인식하도록 설정해야 함.
	if (OverlappedComp == pCubeComponent && OtherActor->IsA(AAB_Character::StaticClass()) && bFromSweep)
	{
		
		AB2LOG(Warning, TEXT("%s Begins Overlap Event!"), *OverlappedComp->GetName());
	}
}

void AAB_SoundCube_2::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (true)
	{
		// AB2LOG(Warning, TEXT("~ Begins Overlap Event!"))
	}
}
