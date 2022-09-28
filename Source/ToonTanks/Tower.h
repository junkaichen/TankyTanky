// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */

UENUM()
enum Direction {Up, Down, Left, Right, DownLeft, DownRight};

UENUM(BlueprintType)


	enum TowerType 
	{
		Guard	UMETA(DisplayName="Guard"),
		Normal UMETA(DisplayName="Normal"),  
		Speed UMETA(DisplayName="Speed")
	};



UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	int32 GetTowerType();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	void RandomFire();
	void RandomDirection();

	TArray<Direction> direction = {Up, Down, Left, Right, DownLeft, DownRight, Up, Down};

private:
	// Player Pawn
	class ATank* Tank;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 0.f;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<TowerType> MyTowerType;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxTurnTime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MinTurnTime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float FireRate = 2.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float BeginRandomFireRate = 2.f;

	FTimerHandle FireRateTimerHandle;
	FTimerHandle ChangeDirectionTimerHandle;
	FTimerHandle RandomFireTimerHandle;

	bool ChangingDirection;
	
	float MoveDirection = 1.f;
	float RotateDirection = 1.f;


	void CheckFireCondition();
	bool InFireRange();


	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, 
	AActor* otherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	


};
