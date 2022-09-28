// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"


UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();
	virtual void Tick(float DeltaTime) override;
	void HandleDestruction();
	void Fire();

protected:
	
	void RandomRotateTurret();
	void RotateTurret(FVector LookAtTarget) ;
	
	void HoldFire();
	AActor* MyProjectile;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;



private: 





	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* DeathParticles;
	
	/* Allows us to have a C++ variable that represents a class type,
	and the type can be based on a Blueprint*/


	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* DeathSound;



	UPROPERTY(EditAnywhere, Category = "FX")
	TSubclassOf<AActor> ToSpawn;

	FTimerHandle RandomRotateTurretHandle;
	


	

};
