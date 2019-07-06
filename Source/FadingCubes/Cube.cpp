// Fill out your copyright notice in the Descript`ion page of Project Settings.


#include "Cube.h"
#include "CubesController.h"

// Sets default values
ACube::ACube()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	this->RootComponent = mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> o(TEXT("/Engine/BasicShapes/Cube.Cube"));
	mesh->SetStaticMesh(o.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> m(TEXT("/Game/Cube_Shader_Inst.Cube_Shader_Inst"));
	material = m.Object;

	OnClicked.AddDynamic(this, &ACube::OnClickedListener);
}

// Called when the game starts or when spawned
void ACube::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		if (UKismetSystemLibrary::GetDisplayName(Actor) == TEXT("CubesController"))
		{
			cubesController = Cast<ACubesController> (Actor);
			break;
		}
	}

	dynamicMatInstance = UMaterialInstanceDynamic::Create(material, this);
	if (dynamicMatInstance) {
		dynamicMatInstance->SetVectorParameterValue(FName("Diffuse"), FColor::Black);

		mesh->SetMaterial(0, dynamicMatInstance);
	}
}

void ACube::OnClickedListener(AActor* touchedActor, FKey buttonPressed)
{
	FLinearColor returnedColor = FLinearColor::Black;
	dynamicMatInstance->GetVectorParameterValue(FName("Diffuse"), returnedColor);

	if (cubesController->LeastCubes.Contains(this)) {
		cubesController->LeastCubes.Remove(this);

		isThrowedAway = true;
	} else {
		FVector centerPoint = FVector(0, (cubesController->CurrentY - 1) * 12 * 100 + 180, 240);

		for (int i = 0; i < 20; i++) {
			FVector direction = (cubesController->Cubes[i]->GetActorLocation() - centerPoint) + FVector(0, FMath::FRandRange(-100.0f, 100.0f), FMath::FRandRange(-100.0f, 100.0f));
			direction.Normalize();
			cubesController->Cubes[i]->explodingDirection = direction;
			cubesController->Cubes[i]->isExploding = true;
		}

		FTimerDelegate TimeDelegate = FTimerDelegate::CreateUObject(this, &ACube::Reload);
		FTimerHandle TimeHandler;

		GetWorldTimerManager().SetTimer(TimeHandler, TimeDelegate, 1.0f, false);
	}
}

void ACube::Reload()
{
	UGameplayStatics::OpenLevel(this->GetWorld(), "/Game/FadjngCubes_Map.FadjngCubes_Map");
}

// Called every frame
void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isThrowedAway)
		SetActorLocation(GetActorLocation() + FVector(100 * 100 * DeltaTime, 0, 0));

	if (isExploding)
		SetActorLocation(GetActorLocation() + explodingDirection * 20 * 100 * DeltaTime);
}

// Called to bind functionality to input
void ACube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

