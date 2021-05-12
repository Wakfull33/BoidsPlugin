// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "Components/SphereComponent.h"
#include "BoidManager.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABoid::ABoid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//StaticMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABoid::BeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABoid::EndOverlap);


}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();

	CachedPos = GetActorLocation();
	CachedDirection = GetActorForwardVector();

}

void ABoid::Initialize()
{
	SetActorLocation(CachedPos);
	SetActorRotation(CachedDirection.Rotation());

	SphereComponent->SetSphereRadius(Manager->Settings.BoundRadius);

	//StaticMesh->SetStaticMesh(Manager->BoidMesh);

	//StaticMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	//StaticMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	//StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	float StartSpeed = (Manager->Settings.MinSpeed + Manager->Settings.MaxSpeed) / 2;
	Velocity = GetActorForwardVector() * StartSpeed;
}

void ABoid::UpdateBoid()
{	
	TargetPos = Manager->Target;
	if(Manager->ShowTarget)
		DrawDebugSphere(GetWorld(), TargetPos, 16.0f, 12, FColor::Green);
	if(Manager->ShowBoids)
		DrawDebugCone(GetWorld(), GetActorLocation(), -GetActorForwardVector(), 10.0f, 0.33f, 0.33f, 12, FColor::Red);

	//UE_LOG(LogTemp, Warning, TEXT("%s is Updating"), *GetName())
	FVector AccelerationVect = FVector::ZeroVector;

	FVector OffsetToTarget = TargetPos - GetActorLocation();
	AccelerationVect = SteeringToward(OffsetToTarget) * Manager->Settings.TargetWeight;

	if (NumFlockmates > 0) {
		FlockingGroupCenter /= NumFlockmates;

		FVector OffsetToFlockingGroupCenter = (FlockingGroupCenter - GetActorLocation());

		FVector AlignmentForce = SteeringToward(AverageFlockHeading) * Manager->Settings.AlignWeight;
		FVector CohesionForce = SteeringToward(OffsetToFlockingGroupCenter) * Manager->Settings.CohesionWeight;
		FVector SeparateForce = SteeringToward(AverageAvoidHeading) * Manager->Settings.SeparateWeight;

		AccelerationVect += (AlignmentForce + CohesionForce + SeparateForce);
	}

	if (IsGoingToCollide) {
		FVector CollisionAvoidDirection = AvoidanceDirection();
		FVector CollisionAvoidForce = SteeringToward(CollisionAvoidDirection) * Manager->Settings.AvoidCollisionWeight;
		AccelerationVect += CollisionAvoidForce;
	}

	Velocity += AccelerationVect * GetWorld()->DeltaTimeSeconds;

	float Speed = Velocity.Size();
	FVector Direction = Velocity / Speed;
	Speed = FMath::Clamp<float>(Speed, Manager->Settings.MinSpeed, Manager->Settings.MaxSpeed);
	Velocity = Direction * Speed;

	CachedPos += Velocity * GetWorld()->DeltaTimeSeconds;
	CachedDirection = Direction;

	SetActorLocation(CachedPos);
	SetActorRotation(CachedDirection.Rotation());
}

FVector ABoid::AvoidanceDirection()
{
	TArray<FVector> Directions = Manager->DetectionDirections;

	for (int i = 0; i < Directions.Num(); ++i) {
		FVector Direction = Directions[i];

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), GetActorLocation() + (Direction * Manager->Settings.CollisionAvoidDistance), Manager->DetectionChannel);
		if (Manager->ShowBoidsDetection) {
			FColor DebugColor = Hit.bBlockingHit ? FColor::Red : FColor::Green;
			
			DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (Direction * Manager->Settings.CollisionAvoidDistance), DebugColor);
		}


		if (!Hit.bBlockingHit)
			return Direction;
	}

	return Forward;
}

FVector ABoid::SteeringToward(FVector Toward)
{
	FVector Vector = Toward.GetSafeNormal() * Manager->Settings.MaxSpeed - Velocity;

	return Vector.GetClampedToSize(0, Manager->Settings.MaxSteerForce);
}

void ABoid::BeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IsGoingToCollide = true;
}

void ABoid::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IsGoingToCollide = false;
}