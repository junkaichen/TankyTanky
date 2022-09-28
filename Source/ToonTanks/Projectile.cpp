// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Tank.h"
#include "Tower.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Diamond.h"
#include "Camera/CameraShakeBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "ToonTanksPlayerController.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	
	ProjectileSmokeTrailComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	ProjectileSmokeTrailComponent->SetupAttachment(RootComponent);
	

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	// OnComponentHit(Hit Event) is a multicast delegate
	// Function OnHit will be added to the Hit Events Invocation list
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);


	/* Parameter:
	World Context Object,
	Sound,
	Location */
	
	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());

	
	
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// Callback function for Hitevent (must have specific parameter list, function name can be DIY)
/*Parameter:
Component that start the Hit 
Actor that receive the Hit
Component that receive the Hit
..
..*/
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(GetOwner() != nullptr)
	{
		// Get the Owner of the Projectile (usually Pawn)
		auto MyOwner = GetOwner();
		// if(OtherActor->FindComponentByClass(HealthComp))
		// {UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
		// UE_LOG(LogTemp, Display, TEXT("222222"));}

		// Get the Instigator from the Pawn
		auto MyOwnerInstigator = MyOwner->GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();
		if(OtherActor && OtherActor != this && OtherActor != MyOwner)
		{
			// Generate Damage Event, DamageTaken() in HealthComponent will be called 
			// These input parameters will pass to the DamageTaken() funciton
			/*Parameter:
			Actor taking the damage
			Damage amount
			Damage Types (Fire, poision, etc)
			An instigator is the controller responsible for the damage
			Actor causing the damage*/
			if(!(MyOwner->IsA(ATower::StaticClass()) && OtherActor->IsA(ATower::StaticClass())))
			{
				UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
			}

			if(OtherActor->IsA(ADiamond::StaticClass()))
			{
				Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
				UGameplayStatics::ApplyDamage(Tank, Damage, MyOwnerInstigator, this, DamageTypeClass);
			}
			// if(MyOwner->IsA(ATank::StaticClass()))
			// {
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());

				
			// }
			if(FrozePlayer && OtherActor->IsA(ATank::StaticClass()))
			{
				Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
				Tank->Froze();
			}

			if(HitCameraShakeClass && OtherActor->IsA(ATank::StaticClass()))
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
				UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

				
			}
			/* Parameter:
			World Context Object,
			Particle System,
			Location,
			Rotation */

			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
			Destroy();
		}
	}
	
	Destroy();
}


int32 AProjectile::GetTotalAmmo()
{
	return Ammo;
}

float AProjectile::GetAmmoReloadTime()
{
	return AmmoReloadTime;
}

USoundBase* AProjectile::GetLaunchSound()
{
	return LaunchSound;
}