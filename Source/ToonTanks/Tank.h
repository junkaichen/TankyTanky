// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Savable")
	int32 TotalPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Savable")
	int32 LevelStage;

	// Switch Levels
	void SwitchLevel();
	TArray<FString> Levels;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int32 Ammo;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	int32 FullAmmo;


	ATank();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();
	void Fire();
	void Froze();
	void UnFroze();
	APlayerController* GetTankPlayerController() const {return TankPlayerController;};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UAudioComponent* AudioPlayer;

	
	void Move(float Value);
	void Turn(float Value);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 0.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;
	// APlayerController is a child class of AController
	APlayerController* TankPlayerController;

	FTimerHandle ReloadAmmoHandle;
	FTimerHandle FrozeTimeHandle;

	void ReloadAmmo();

	class AProjectile* CurrentProjectile;

	void Reload();

	bool IsFroze = false;


	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AActor> FrozeEmitter;

};
