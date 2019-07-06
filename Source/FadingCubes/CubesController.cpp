// Fill out your copyright notice in the Description page of Project Settings.


#include "CubesController.h"
#include "Cube.h"

ACubesController::ACubesController()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACubesController::BeginPlay()
{
	Super::BeginPlay();	

	PlayerStartLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	FTimerDelegate TimeDelegate = FTimerDelegate::CreateUObject(this, &ACubesController::GenerateCubes, FVector(0, CurrentY * 12 * 100, 0));
	FTimerHandle TimeHandler;

	GetWorldTimerManager().SetTimer(TimeHandler, TimeDelegate, 0.1f, false);
}

void ACubesController::AssignMostColors()
{
	for (int i = 0; i < 20; i++) {
		Cubes[i]->dynamicMatInstance->SetVectorParameterValue(TEXT("Diffuse"), MostColor);
	}
}

void ACubesController::AssignLeastColors()
{
	for (int i = 0; i < 5;) {
		int32 Random = FMath::RandRange(0, 20 - 1);

		FLinearColor ReturnedColor = FColor::Black;
		Cubes[Random]->dynamicMatInstance->GetVectorParameterValue(TEXT("Diffuse"), ReturnedColor, false);

		if (ReturnedColor == MostColor) {
			Cubes[Random]->dynamicMatInstance->SetVectorParameterValue(TEXT("Diffuse"), LeastColor);
			LeastCubes.Add(Cubes[Random]);
			i++;
		}
	}
}

void ACubesController::GenerateCubes(FVector Origin)
{
	for (int z = 0; z < 5; z++) {
		for (int y = 0; y < 4; y++) {
			ACube* Cube = this->GetWorld()->SpawnActor<ACube>(ACube::StaticClass(), Origin + FVector(0, y * 120, z * 120), FRotator::ZeroRotator);
			Cube->SetLifeSpan(10);
			Cubes.Add(Cube);
		}
	}

	AssignMostColors();
	AssignLeastColors();

	CurrentY++;
}

void ACubesController::RegenerateCubes(FVector Origin)
{
	for (int i = 0; i < 20; i++) {
		Cubes[i]->SetLifeSpan(10);
		Cubes[i] = nullptr;
	}

	bIsSwiping = true;

	for (int z = 0, i = 0; z < 5; z++) {
		for (int y = 0; y < 4; y++, i++) {
			ACube* Cube = this->GetWorld()->SpawnActor<ACube>(ACube::StaticClass(), Origin + FVector(0, y * 120, z * 120), FRotator::ZeroRotator);
			Cubes[i] = Cube;
		}
	}

	AssignMostColors();
	AssignLeastColors();

	CurrentY++;
}

FLinearColor ACubesController::LerpColor(FLinearColor From, FLinearColor To, float Alpha)
{
	return From + Alpha * (To - From);
}

void ACubesController::FadeLeastColorsOut(float DeltaTime)
{
	for (ACube* c : LeastCubes) {
		FLinearColor ReturnedColor = FColor::Black;
		c->dynamicMatInstance->GetVectorParameterValue(TEXT("Diffuse"), ReturnedColor, false);

		FLinearColor Lerp = ACubesController::LerpColor(ReturnedColor, MostColor, DeltaTime * 0.5f);
		c->dynamicMatInstance->SetVectorParameterValue(TEXT("Diffuse"), Lerp);
	}
}

// Called every frame
void ACubesController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LeastCubes.Num() <= 0 && Cubes.Num() == 20) {
		RegenerateCubes(FVector(0, CurrentY * 12 * 100, 0));
	}

	FadeLeastColorsOut(DeltaTime);

	if (bIsSwiping) {
		APawn* player = GetWorld()->GetFirstPlayerController()->GetPawn();
		player->SetActorLocation(FMath::Lerp(player->GetActorLocation(), PlayerStartLocation + FVector(0, (CurrentY - 1) * 12 * 100, 0), GetWorld()->DeltaTimeSeconds * 5));
	}
}

