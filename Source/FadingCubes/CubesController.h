// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "CubesController.generated.h"

class ACube;

UCLASS()
class FADINGCUBES_API ACubesController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubesController();

	UPROPERTY(EditAnywhere)
	TArray<ACube*> Cubes;

	UPROPERTY(EditAnywhere)
	FLinearColor MostColor;
	UPROPERTY(EditAnywhere)
	FLinearColor LeastColor;

	UPROPERTY(EditAnywhere)
	TArray<ACube*> LeastCubes;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerController> player;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void AssignMostColors();
	void AssignLeastColors();

	void GenerateCubes(FVector Origin);
	void RegenerateCubes(FVector Origin);

	FLinearColor LerpColor(FLinearColor From, FLinearColor To, float Alpha);
	void FadeLeastColorsOut(float DeltaTime);

	bool bIsSwiping;

	FVector PlayerStartLocation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	int CurrentY;

};
