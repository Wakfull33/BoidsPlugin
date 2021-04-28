// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Containers/Array.h"
#include "BoidUtils.generated.h"

/**
 * 
 */
UCLASS()
class BOIDSPLUGIN_API UBoidUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
};

USTRUCT(BlueprintType)
struct FBoidSettings {
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Speed")
	float MinSpeed = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Speed")
	float MaxSpeed = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Speed")
	float PerceptionRadius = 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Speed")
	float AvoidanceRadius = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Speed")
	float MaxSteerForce = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Alignment Weight")
	float AlignWeight = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Alignment Weight")
	float CohesionWeight = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Alignment Weight")
	float SeparateWeight = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Alignment Weight")
	float TargetWeight = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Collision")
	float BoundRadius = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Collision")
	float AvoidCollisionWeight = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Collision")
	float CollisionAvoidDistance = 5;

};

USTRUCT(BlueprintType)
struct FBoidData {
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Direction;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FlockHeading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FlockCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SeparationHeading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumFlocking;
};