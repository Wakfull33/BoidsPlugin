// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"
#include "Boid.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABoidManager::ABoidManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();

	SetupDirections();
	Boids.SetNum(SpawnCount);

	for (int i = 0; i < SpawnCount; ++i) {
		FVector Pos = GetActorLocation() + (UKismetMathLibrary::RandomUnitVector() * UKismetMathLibrary::RandomFloatInRange(0, SpawnRadius));
		FRotator Rotation = UKismetMathLibrary::RandomUnitVector().Rotation();
		
		Boids[i] = GetWorld()->SpawnActor<ABoid>(BoidToSpawn, Pos, Rotation);
		Boids[i]->Manager = this;
		Boids[i]->Initialize();
	}	
}

// Called every frame
void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ShowSpawnRadius)
		DrawDebugSphere(GetWorld(), GetActorLocation(), SpawnRadius, 16, FColor::Green);

	if (Boids.Num() > 0) {
		for (int i = 0; i < Boids.Num(); ++i) {
			Calculate();
			Boids[i]->UpdateBoid();
		}
	}

}

void ABoidManager::SetupDirections() 
{
	DetectionDirections.SetNum(DetectionCount);

	float AngleIncrement = PI * 2 * UE_GOLDEN_RATIO;
	//UE_LOG(LogTemp, Warning, TEXT("==========================================="))
	//UE_LOG(LogTemp, Warning, TEXT("AngleIncrement: %f"), AngleIncrement)

	for (int i = 0; i < DetectionCount; ++i)
	{
		float T = (float)i / DetectionCount;
		float Inclination = FMath::Acos(1 - 2 * T);
		float Azimuth = AngleIncrement * i;

		//UE_LOG(LogTemp, Warning, TEXT("[%d]T: %f    Inclination: %f    Azimuth: %f"), i, T, Inclination, Azimuth)

		float x = FMath::Sin(Inclination) * FMath::Cos(Azimuth);
		float y = FMath::Sin(Inclination) * FMath::Sin(Azimuth);
		float z = FMath::Cos(Inclination);

		//UE_LOG(LogTemp, Warning, TEXT("[%d]Direction: (x: %f, y: %f, z: %f"), i, x, y ,z)

		DetectionDirections[i] = FVector(x, y, z);
		//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (DetectionDirections[i] * 100), FColor::Orange, false, 60);

	}
	//UE_LOG(LogTemp, Warning, TEXT("==========================================="))
}

void ABoidManager::Calculate()
{
	//TArray<ABoid> Boids;
	//int NumBoids;
	//float ViewRadius;
	//float AvoidRadius;

	for (int i = 0; i < Boids.Num(); ++i) {
		int TmpFlockmates = 0;
		FVector TmpAverageFlockHeading;
		FVector TmpFlockingGroupCenter;

		for (int j = 0; j < Boids.Num(); ++j) {
			if (i != j) {
				FVector Offset = Boids[j]->GetActorLocation() - Boids[i]->GetActorLocation();
				float SqrtDist = Offset.SizeSquared();

				UE_LOG(LogTemp, Warning, TEXT("[%d] -> [%d] = %f    Perc: %f"), i, j , SqrtDist, Boids[i]->Manager->Settings.PerceptionRadius)

				if (SqrtDist < FMath::Pow(Boids[i]->Manager->Settings.PerceptionRadius, 2)) {
					TmpFlockmates += 1;
				    TmpAverageFlockHeading += Boids[j]->GetActorForwardVector();;
					TmpFlockingGroupCenter += Boids[j]->GetActorLocation();

					if (ShowConnections && i == 0) {
						DrawDebugLine(GetWorld(), Boids[i]->GetActorLocation(), Boids[j]->GetActorLocation(), FColor::Green);
					}

					if (SqrtDist < FMath::Pow(Boids[i]->Manager->Settings.AvoidanceRadius, 2)) {
						//Boids[i].
					}
				}
			}
		}

		Boids[i]->NumFlockmates = TmpFlockmates;
		Boids[i]->AverageFlockHeading = TmpAverageFlockHeading;
		Boids[i]->FlockingGroupCenter = TmpFlockingGroupCenter;
	}
	
}

