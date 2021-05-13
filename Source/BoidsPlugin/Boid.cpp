// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BoidManager.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(BoidLog);

// Sets default values
ABoid::ABoid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//StaticMesh = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(SphereComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	Mesh->SetCollisionProfileName(FName("Boid"));
	Mesh->SetCollisionProfileName(FName("Boid"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(SphereComponent);


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

	FHitResult CollisionHit;
	FCollisionShape SphereShape;
	SphereShape.ShapeType = ECollisionShape::Sphere;
	SphereShape.SetSphere(Manager->Settings.AvoidanceRadius);
	GetWorld()->SweepSingleByChannel(CollisionHit, GetActorLocation(), GetActorLocation() + 0.01f, GetActorRotation().Quaternion(), ECC_Visibility, SphereShape);

	if (CollisionHit.bBlockingHit) {
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