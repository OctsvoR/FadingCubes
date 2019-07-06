// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/Material.h"
#include "EngineUtils.h" 
#include "Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Cube.generated.h"

class ACubesController;

UCLASS()
class FADINGCUBES_API ACube : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACube();

	UStaticMeshComponent* mesh;

	UMaterialInstance* material;
	UMaterialInstanceDynamic* dynamicMatInstance;

	UPROPERTY(EditAnywhere)
	ACubesController* cubesController;

	FVector explodingDirection;

	bool isThrowedAway;
	bool isExploding;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnClickedListener(AActor* touchedActor, FKey buttonPressed);

	void Reload();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
