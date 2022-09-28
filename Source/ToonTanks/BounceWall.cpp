// Fill out your copyright notice in the Description page of Project Settings.


#include "BounceWall.h"
#include "Projectile.h"
#include "Tower.h"
// Sets default values for this component's properties
UBounceWall::UBounceWall()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBounceWall::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UBounceWall::DamageTaken);

	// ...
	
}


// Called every frame
void UBounceWall::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UBounceWall::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if(DamageCauser->GetOwner()->IsA(ATower::StaticClass()))
	{
		ATower* MyTower = Cast<ATower>(DamageCauser->GetOwner());
		int32 MyTowerType = MyTower->GetTowerType();
		if(MyTowerType == 1)
		{
			auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileClass2, 
			DamageCauser->GetActorLocation() + FVector(BounceFace, 0.f, 0.f), 
			FRotator(0.f, 180.f - DamageCauser->GetActorRotation().Yaw , 0.f)
			);
			Projectile->SetOwner(DamageCauser->GetOwner());
		}
		else if (MyTowerType == 0)
		{
			auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileClass, 
			DamageCauser->GetActorLocation() + FVector(BounceFace, 0.f, 0.f), 
			FRotator(0.f, 180.f - DamageCauser->GetActorRotation().Yaw , 0.f)
			);
			Projectile->SetOwner(DamageCauser->GetOwner());
		}
		
	}
	else
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass, 
		DamageCauser->GetActorLocation() + FVector(BounceFace, 0.f, 0.f), 
		FRotator(0.f, 180.f - DamageCauser->GetActorRotation().Yaw , 0.f)
		);
		Projectile->SetOwner(DamageCauser->GetOwner());
	}
	
	// The Actor that shot the fire will become Owner
	
}