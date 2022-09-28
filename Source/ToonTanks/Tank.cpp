// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "Projectile.h"
#include "HealthComponent.h"
#include "Camera/CameraShakeBase.h"

ATank::ATank()
{
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArmComp->SetupAttachment(RootComponent);
    AudioPlayer = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Player"));
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp);


    Levels.Add("Level_1");
    Levels.Add("Level_2");
    Levels.Add("Level_3");
    Levels.Add("Level_4");
    Levels.Add("Level_5");
    Levels.Add("Level_6");
    Levels.Add("Level_7");
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
    // GetController() return AController
    // APlayerController is a child class of AController, so we casting AController into APlayerController
    TankPlayerController = Cast<APlayerController>(GetController());

    CurrentProjectile = ProjectileClass.GetDefaultObject();
	Ammo = CurrentProjectile->GetTotalAmmo();
    FullAmmo = Ammo;

}

// Binding function to Axis Mapping
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // we are calling the parent version of this function
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Binding function to Axis Mapping
    /* Parameter: 
    Name of the Axis Mapping, 
    A pointer to the object where binding the function for
    The address of the function we're binding
    */ 
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

    // Binding function to Action Mapping
    /* Parameter: 
    Name of the Action Mapping, 
    When Press the button or Release the button
    A pointer to the object where binding the function for
    The address of the function we're binding
    */ 
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
    PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ATank::Reload);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ATank::HoldFire);
}

void ATank::SwitchLevel()
{
    FString currentLevel = GetWorld()->GetMapName();
    currentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
    UGameplayStatics::OpenLevel(GetWorld(), FName(Levels[Levels.Find(currentLevel) + 1]));
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if( TankPlayerController)
    {
        FHitResult HitResult;
        // Performing a line trace (End point is the mouse cursor)
        // Get the Hit result under the cursor
         TankPlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility, 
            false, 
            HitResult);
        RotateTurret(HitResult.ImpactPoint);
    }


}

void ATank::Move(float Value)
{
    if(!IsFroze )
    {
        FVector DeltaLocation = FVector::ZeroVector;
        // X = Value * DeltaTime * Speed
        DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
        AddActorLocalOffset(DeltaLocation, true);
    }
    
}

void ATank::Turn(float Value)
{
    if(!IsFroze)
    {
        FRotator DeltaRotation = FRotator::ZeroRotator;
        // Yaw = Value * DeltaTime * TurnRate
        // Yaw means Rotation around the Z axis
        DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
        AddActorLocalRotation(DeltaRotation, true);
    }

    
}


void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    if(DeathCameraShakeClass)
    {
        GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
    }
    
    // Hide the Tank
    SetActorHiddenInGame(true);
    // Disable Tank's movement
    // SetActorTickEnabled(false);
}


void ATank::Fire()
{
    // Ammo is enough, Fire
    if(Ammo > 0 && !GetWorldTimerManager().IsTimerActive(ReloadAmmoHandle))
    {
        Ammo--;
        // Invoke call Fire() in Base Class 
        // UGameplayStatics::PlaySoundAtLocation(this, CurrentProjectile->GetLaunchSound(), GetActorLocation());
        
        Super::Fire();
    } 
    // Ran out of Ammo
    if(Ammo == 0 && !GetWorldTimerManager().IsTimerActive(ReloadAmmoHandle))
    {
        AudioPlayer->Play();
        GetWorldTimerManager().SetTimer(
            ReloadAmmoHandle, 
            this, 
            &ATank::ReloadAmmo, 
            CurrentProjectile->GetAmmoReloadTime(), 
            false);
    }
    
    
    
}


void ATank::ReloadAmmo()
{
    Ammo = FullAmmo;
    AudioPlayer->Stop();
}

void ATank::Reload()
{
    if(Ammo != FullAmmo && !GetWorldTimerManager().IsTimerActive(ReloadAmmoHandle))
    {
        AudioPlayer->Play();
            GetWorldTimerManager().SetTimer(
            ReloadAmmoHandle, 
            this, 
            &ATank::ReloadAmmo, 
            CurrentProjectile->GetAmmoReloadTime(), 
            false);
    }

    

}

void ATank::Froze()
{
    GetWorldTimerManager().ClearTimer(FrozeTimeHandle);
    IsFroze = true;
    GetWorldTimerManager().SetTimer(FrozeTimeHandle, this, &ATank::UnFroze, 2.f, false);
    GetWorld()->SpawnActor<AActor>(
		FrozeEmitter, 
		GetActorLocation(), 
		GetActorRotation()
	);
}

void ATank::UnFroze()
{
    IsFroze = false;
}
