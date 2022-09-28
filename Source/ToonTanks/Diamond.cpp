// Fill out your copyright notice in the Description page of Project Settings.


#include "Diamond.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ADiamond::ADiamond()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DiamondMesh = CreateDefaultSubobject<UStaticMeshComponent>("Diamond Mesh");
	RootComponent = DiamondMesh;
	NiagaraDiamond = CreateDefaultSubobject<UNiagaraComponent>("Niagara Diamond");

	NiagaraDiamond->SetupAttachment(DiamondMesh);

}

// Called when the game starts or when spawned
void ADiamond::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADiamond::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADiamond::HandleDestruction()
{
	
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	Destroy();
}