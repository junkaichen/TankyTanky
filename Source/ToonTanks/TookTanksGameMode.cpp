// Fill out your copyright notice in the Description page of Project Settings.


#include "TookTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "Diamond.h"
#include "ToonTanksPlayerController.h"


void ATookTanksGameMode::ActorDied(AActor* DeadActor)
{
    // Destruct Tank
    if(DeadActor == Tank)
    {
        Tank->HandleDestruction();
        // Disable PlayerController input and stop showing the Targeting cursor
        if(ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
        
    }
    // Destruct Tower
    else if(ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        TargetTowers--;
        if(TargetTowers == 0)
        {
            GameOver(true);
            if(ToonTanksPlayerController)
            {
                ToonTanksPlayerController->SetPlayerEnabledState(false);
            }
            // Tank->SwitchLevel();
        }
    }
    else if(ADiamond* DestroyedDiamond = Cast<ADiamond>(DeadActor))
    {
        DestroyedDiamond->HandleDestruction();
        
        if(ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    }
    else
    {
        DeadActor->Destroy();
    }
}

void ATookTanksGameMode::BeginPlay()
{
    Super::BeginPlay();
    HandleGameStart();
    IsStarted = false;
    TargetTowers = 20;
    NumberOfRespawner = 4;
    // PlayerInputComponent->BindAction(TEXT("Skip"), IE_Pressed, this, &ATookTanksGameMode::SkipMovie);
    
    // GetWorldTimerManager().SetTimer(MovieTimerHandle, this, &ATookTanksGameMode::HandleGameStart, 20.f, false);

}

void ATookTanksGameMode::HandleGameStart()
{
    
    // Get the Controller
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    
    
    
    GetWorldTimerManager().SetTimer(MovieTimerHandle, this, &ATookTanksGameMode::StartGame, 15.f, false);

    if(ToonTanksPlayerController)
    {
        // At the beginning of the game, Player can't move the Tank
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        

        /* Parameters: 
        A user object which the callback function exists on
        Address of the callback function
        input values that we would like to passed into the callback function*/
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController, 
            &AToonTanksPlayerController::SetPlayerEnabledState, 
            true
        );

        /* Parameters:
        Timer Handle,
        Timer Delegate,
        Play Rate,
        Looping or not */
        GetWorldTimerManager().SetTimer(
            PlayerEnableTimerHandle, 
            PlayerEnableTimerDelegate, 
            StartDelay, 
            false
        );
    }
    
}


int32 ATookTanksGameMode::GetTargetTowerCount()
{
    /* Parameters:
    World Conetext Object,
    TSubclassOf<AActor>, this can be a UClass*,
    TArray<AActor*> */
    TArray<AActor*> Towers;
    // TArray we passed in will also be the output parameter
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}

void ATookTanksGameMode::SkipMovie()
{
    if(!IsStarted)
    {
        GetWorldTimerManager().ClearTimer(MovieTimerHandle);
        GetWorldTimerManager().ClearTimer(PlayerEnableTimerHandle);
        StartGame();
        Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
        ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        

        /* Parameters: 
        A user object which the callback function exists on
        Address of the callback function
        input values that we would like to passed into the callback function*/
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController, 
            &AToonTanksPlayerController::SetPlayerEnabledState, 
            true
        );

        /* Parameters:
        Timer Handle,
        Timer Delegate,
        Play Rate,
        Looping or not */
        GetWorldTimerManager().SetTimer(
            PlayerEnableTimerHandle, 
            PlayerEnableTimerDelegate, 
            3.f, 
            false
        );
        IsStarted = true;
    }
    
}