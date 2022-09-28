// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleTurretTower.h"
#include "Components/SceneComponent.h"
#include "Projectile.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"



void ADoubleTurretTower::BeginPlay()
{
    Super::BeginPlay();
    // Our Tank set to be Player 0 in Unreal Engine
    // Get Tank in Tower class
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle2, this, &ADoubleTurretTower::CheckFireCondition, FireRate, true);
 
    
}







ADoubleTurretTower::ADoubleTurretTower()
{
    TurretMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh3"));
    TurretMesh3->SetupAttachment(TurretMesh);
    TurretMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh2"));
    TurretMesh2->SetupAttachment(TurretMesh);
    ProjectileSpawnPoint2 = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point2"));
    ProjectileSpawnPoint2->SetupAttachment(TurretMesh);
}





void ADoubleTurretTower::Fire()
{
    // Super::Fire();
    // Function SpawnActor<>() Parameter: <require a type>(UClass, Location, Rotation)
    auto Projectile = GetWorld()->SpawnActor<AProjectile>(
    ProjectileClass, 
    ProjectileSpawnPoint2->GetComponentLocation(), 
    ProjectileSpawnPoint2->GetComponentRotation()
    );
    // The Actor that shot the fire will become Owner
    Projectile->SetOwner(this);
}



void ADoubleTurretTower::CheckFireCondition()
{
    if(InFireRange())
    {
        Fire();
    }
}




bool ADoubleTurretTower::InFireRange()
{
    if(Tank)
    {
        float Distance = FVector::Dist(Tank->GetActorLocation(), GetActorLocation());
        if(Distance <= 2000.f)
        {
            return true;
        }
    }
    return false;
}