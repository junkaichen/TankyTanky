// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"

void ATower::BeginPlay()
{
    Super::BeginPlay();
    // Our Tank set to be Player 0 in Unreal Engine
    // Get Tank in Tower class
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    /* Parameter: 
    Timer Handle
    The User class(object) that we're binding the function for
    callback function 
    How much time should pass before the callback function gets called
    whether the Timer should loop or not
    */ 
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);

 
    
    

    CapsuleComp->OnComponentHit.AddDynamic(this, &ATower::OnHit);
    RandomDirection();
    GetWorldTimerManager().SetTimer(RandomFireTimerHandle, this, &ATower::RandomFire, BeginRandomFireRate, false);
    
}





void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // RandomRotateTurret();
    // Check to see if the Tank is in range
    // If in range, rotate turret toward Tank
    if(InFireRange())
    {
        RotateTurret(Tank->GetActorLocation());
    }
    FVector DeltaLocation = FVector::ZeroVector;
    // X = Value * DeltaTime * Speed
    DeltaLocation.X = MoveDirection * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation, true);


    FRotator DeltaRotation = FRotator::ZeroRotator;
    DeltaRotation.Yaw = RotateDirection * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
    
}

void ATower::CheckFireCondition()
{
    if(InFireRange())
    {
        GetWorldTimerManager().ClearTimer(RandomFireTimerHandle);
        Fire();
        if(BeginRandomFireRate < 100)
        {
            GetWorldTimerManager().SetTimer(RandomFireTimerHandle, this, &ATower::RandomFire, FireRate, false);
        }
        
    }
}

void ATower::RandomFire()
{
    Fire();
    // GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::Fire, 0.5f, false);


    float RandomFireRate = FMath::RandRange(1.f, 3.f);
    GetWorldTimerManager().SetTimer(RandomFireTimerHandle, this, &ATower::RandomFire, RandomFireRate, false);
}

bool ATower::InFireRange()
{
    if(Tank)
    {
        float Distance = FVector::Dist(Tank->GetActorLocation(), GetActorLocation());
        if(Distance <= FireRange)
        {
            return true;
        }
    }
    return false;
}

void ATower::HandleDestruction()
{
    // HandleDestruction() in the parent class(BasePawn.h & BasePawn.cpp) will be called first
    Super::HandleDestruction();

 
    // Destroy the Tower
    Destroy();
}

// AI Tank can Move Randomly in Map
void ATower::RandomDirection()
{
    
    Direction selection = direction[FMath::FloorToInt32(FMath::RandRange(0.f, 7.f))];
    float ChangeInterval = FMath::RandRange(3.f, 10.f);
    float ActualTurnTime = FMath::RandRange(MinTurnTime, MaxTurnTime);
    
    if(selection == Direction::Up)
    {
        MoveDirection = 1.f;
        RotateDirection = 0.f;
        
        GetWorldTimerManager().SetTimer(ChangeDirectionTimerHandle, this, &ATower::RandomDirection, ChangeInterval, true);
    }
    if(selection == Direction::Down)
    {
        MoveDirection = -1.f;
        RotateDirection = 0.f;
        GetWorldTimerManager().SetTimer(ChangeDirectionTimerHandle, this, &ATower::RandomDirection, ChangeInterval, true);

    }
    if(selection == Direction::Right)
    {
        MoveDirection = 1.f;
        RotateDirection = 1.f;
        
        // GetWorldTimerManager().SetTimer(ChangeDirectionTimerHandle, this, &ATower::RandomDirection, FMath::FloorToInt32(ActualTurnTime), true);
        GetWorldTimerManager().SetTimer(ChangeDirectionTimerHandle, this, &ATower::RandomDirection, 1.f, true);
    }
    if(selection == Direction::Left)
    {
        MoveDirection = 1.f;
        RotateDirection = -1.f;
        // GetWorldTimerManager().SetTimer(ChangeDirectionTimerHandle, this, &ATower::RandomDirection, FMath::FloorToInt32(ActualTurnTime), true);
        GetWorldTimerManager().SetTimer(ChangeDirectionTimerHandle, this, &ATower::RandomDirection, 1.f, true);
    }
    if(selection == Direction::DownLeft)
    {
        MoveDirection = -1.f;
        RotateDirection = -1.f;
        // GetWorldTimerManager().SetTimer(ChangeDirectionTimerHandle, this, &ATower::RandomDirection, FMath::FloorToInt32(ActualTurnTime), true);
        GetWorldTimerManager().SetTimer(ChangeDirectionTimerHandle, this, &ATower::RandomDirection, 1.f, true);
    }
    if(selection == Direction::DownRight)
    {
        MoveDirection = -1.f;
        RotateDirection = 1.f;
        // GetWorldTimerManager().SetTimer(ChangeDirectionTimerHandle, this, &ATower::RandomDirection, FMath::FloorToInt32(ActualTurnTime), true);
        GetWorldTimerManager().SetTimer(ChangeDirectionTimerHandle, this, &ATower::RandomDirection, 1.f, true);
    }

    
    
}


void ATower::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // If AI Tank Hit Border, Change diirection immediately 
	if(GetWorldTimerManager().IsTimerActive(ChangeDirectionTimerHandle))
    {
        // Clear the existing ChangeDirection Timer
        GetWorldTimerManager().ClearTimer(ChangeDirectionTimerHandle);
        RandomDirection();
    }
	
}

int32 ATower::GetTowerType()
{
    if(MyTowerType == TowerType::Normal)
    {
        return 0;
    }
    else if(MyTowerType == TowerType::Guard)
    {
        return 1;
    }
    else 
    {
        return 2;
    }
    
}