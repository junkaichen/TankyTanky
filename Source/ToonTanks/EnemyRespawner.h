// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyRespawner.generated.h"

UCLASS()
class TOONTANKS_API AEnemyRespawner : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AEnemyRespawner();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:

	UPROPERTY(EditAnywhere, Category = "Respawning")
	TSubclassOf<class ATower> TowerToSpawn;

	UFUNCTION(BlueprintCallable)
	void SpawnObject();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RespawnPoint;

	UPROPERTY(EditAnywhere, Category = "Respawning")
	int32 TotalRespawnTank;


	UPROPERTY(EditAnywhere, Category = "Respawning")
	float RandomRespawnMinRange = 30;

	UPROPERTY(EditAnywhere, Category = "Respawning")
	float RandomRespawnMaxRange = 40;

	FTimerHandle RandomSpawnHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float BeginCounter = 0.f;

	FTimerHandle BeginCounterHandle;

	class ATookTanksGameMode* MyMode;
};
