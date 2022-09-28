// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "DoubleTurretTower.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ADoubleTurretTower : public ATower
{
	GENERATED_BODY()

	public:

	ADoubleTurretTower();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh3;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Fire();

	void CheckFireCondition();
	bool InFireRange();

	float FireRate = 0.3f;

	class ATank* Tank;

	FTimerHandle FireRateTimerHandle2;
	FTimerHandle ChangeDirectionTimerHandle;
	
};
