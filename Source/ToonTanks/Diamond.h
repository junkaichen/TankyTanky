// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Diamond.generated.h"

UCLASS()
class TOONTANKS_API ADiamond : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADiamond();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DiamondMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* NiagaraDiamond;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void HandleDestruction();


private:
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* DeathParticles;
	
	/* Allows us to have a C++ variable that represents a class type,
	and the type can be based on a Blueprint*/


	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* DeathSound;

};
