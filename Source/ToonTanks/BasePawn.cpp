// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Projectile.h"
// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create Capsule Component
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	// Reassign RootComponent in C++ class
	RootComponent = CapsuleComp;
	// Construct StaticMesh Component for Base Mesh
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	// Attach Base Mesh to CapsuleComponent
	BaseMesh->SetupAttachment(CapsuleComp);
	// Construct StaticMesh Component for Turret Mesh
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	// Attach Turret Mesh to BaseMesh
	TurretMesh->SetupAttachment(BaseMesh);
	// Construct SceneComponent for the ProjectileSpawnPoint
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	// Attach ProjectileSpawnPoint to TurretMesh
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

}	



void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	// a vector from a start point to an end point = End point - Start point 
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	// Convert 
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	TurretMesh->SetWorldRotation(
		// smooth the Rotation
		FMath::RInterpTo(
			// The current Rotation we want to change
			TurretMesh->GetComponentRotation(), 
			// Target Rotation
			LookAtRotation, 
			// The World
			UGameplayStatics::GetWorldDeltaSeconds(this), 
			10.f)
		);

}


void ABasePawn::RandomRotateTurret()
{
	float RotateDegree = FMath::RandRange(60.f, 360.f);
	FRotator LookAtRotation = TurretMesh->GetComponentRotation() + FRotator(0.f, RotateDegree, 0.f);
		TurretMesh->SetWorldRotation(
		// smooth the Rotation
		FMath::RInterpTo(
			// The current Rotation we want to change
			TurretMesh->GetComponentRotation(), 
			// Target Rotation
			LookAtRotation, 
			// The World
			UGameplayStatics::GetWorldDeltaSeconds(this), 
			10.f)
		);
	float ChangeInterval = FMath::RandRange(2.f, 5.f);
    GetWorldTimerManager().SetTimer(RandomRotateTurretHandle, this, &ABasePawn::RandomRotateTurret, ChangeInterval, true);
}

void ABasePawn::Fire()
{
	// Function SpawnActor<>() Parameter: <require a type>(UClass, Location, Rotation)
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass, 
		ProjectileSpawnPoint->GetComponentLocation(), 
		ProjectileSpawnPoint->GetComponentRotation()
	);
	// The Actor that shot the fire will become Owner
	Projectile->SetOwner(this);

	// Laser Gun
	// if(ToSpawn)
	// {
	// 	FActorSpawnParameters spawnParams;
	// 	spawnParams.Owner = this;
	// 	UWorld* world = GetWorld();

	// 	MyProjectile = world->SpawnActor<AActor>(ToSpawn,
	// 	ProjectileSpawnPoint->GetComponentLocation(),
	// 	ProjectileSpawnPoint->GetComponentRotation(),
	// 	spawnParams);

	// }

	
}


// child class should call this parent function too
void ABasePawn::HandleDestruction()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
}


void ABasePawn::HoldFire()
{
	if(MyProjectile)
	{
		GetWorld()->DestroyActor(MyProjectile);
	}
	
}

void ABasePawn::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	
	if(MyProjectile)
	{
		MyProjectile->SetActorLocation(ProjectileSpawnPoint->GetComponentLocation());
		MyProjectile->SetActorRotation(ProjectileSpawnPoint->GetComponentRotation());
	}

}