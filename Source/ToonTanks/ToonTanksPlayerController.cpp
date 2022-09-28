// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"


// Controll the PlayerController input
void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    if(bPlayerEnabled)
    {
        // Access the Pawn that this ToonTanksPlayerController is possessing
        // (the Pawn will be the Toon Tank the Player is controlling)
        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
    }
    //(游戏内的瞄准镜)
    // Set the MouseCursor in game enable or disable
    if(bPlayerEnabled)
    {
        bShowMouseCursor = bPlayerEnabled;
    }
    
}