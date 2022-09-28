// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TookTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATookTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);

	UFUNCTION(BlueprintCallable)
	void SkipMovie();

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool IsStarted;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	int32 TargetTowers;

	int32 NumberOfRespawner;




protected:

	virtual void BeginPlay() override;

	// Only need to declare it in header file,
	// Implementation will in Blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

	UFUNCTION(BlueprintImplementableEvent)
	void PauseGame();
	
private:
	class ATank* Tank;
	class AToonTanksPlayerController* ToonTanksPlayerController;

	int32 GetTargetTowerCount();



	
	float StartDelay = 18.f;

	FTimerHandle MovieTimerHandle;
	FTimerHandle PlayerEnableTimerHandle;

	
	void HandleGameStart();

};
