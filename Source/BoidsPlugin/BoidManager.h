// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidUtils.h"
#include "BoidManager.generated.h"

class ABoid;

UCLASS()
class BOIDSPLUGIN_API ABoidManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int SpawnCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	float SpawnRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<ABoid> BoidToSpawn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boids")
	TArray<ABoid*> Boids;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boids")
	FBoidSettings Settings;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boids")
	FVector Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boids")
	bool ShowConnections = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boids|Visual")
	class UStaticMesh* BoidMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Detection")
	TArray<FVector> DetectionDirections;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Detection")
	TEnumAsByte<ECollisionChannel> DetectionChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Detection")
	int DetectionCount = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool ShowBoids = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool ShowSpawnRadius = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool ShowTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool ShowBoidsDetection = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupDirections();

	void Calculate();

};
