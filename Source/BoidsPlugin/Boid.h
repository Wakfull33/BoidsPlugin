// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidUtils.h"
#include "Boid.generated.h"

UCLASS()
class BOIDSPLUGIN_API ABoid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, Category = "State")
	FVector Position;

	UPROPERTY(VisibleAnywhere, Category = "State")
	FVector Forward;

	UPROPERTY(VisibleAnywhere, Category = "State")
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, Category = "State")
	bool IsGoingToCollide = false;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	FVector Acceleration;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	FVector AverageFlockHeading;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	FVector AverageAvoidHeading;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	FVector FlockingGroupCenter;
	
	UPROPERTY(VisibleAnywhere, Category = "Properties")
	int NumFlockmates;

	UPROPERTY(VisibleAnywhere, Category = "Cache")
	FVector TargetPos;

	UPROPERTY(VisibleAnywhere, Category = "Cache")
	FVector CachedPos;

	UPROPERTY(VisibleAnywhere, Category = "Cache")
	FVector CachedDirection;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
	FBoidSettings BoidSettings;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* SphereComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class ABoidManager* Manager = nullptr;


	UFUNCTION()
	void Initialize();
	
	UFUNCTION()
	void UpdateBoid();

	UFUNCTION()
	FVector AvoidanceDirection();

	UFUNCTION()
	FVector SteeringToward(FVector Toward);

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};