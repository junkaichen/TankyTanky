// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRespawner.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "TookTanksGameMode.h"

// Sets default values
AEnemyRespawner::AEnemyRespawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RespawnPoint = CreateDefaultSubobject<USceneComponent>("Respawn Point");
	RootComponent = RespawnPoint;

}

// Called when the game starts or when spawned
void AEnemyRespawner::BeginPlay()
{
	Super::BeginPlay();
	MyMode = Cast<ATookTanksGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GetWorldTimerManager().SetTimer(RandomSpawnHandle, this, &AEnemyRespawner::SpawnObject, BeginCounter , false);
	
}

// Called every frame
void AEnemyRespawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(MyMode->IsStarted && MyMode->NumberOfRespawner != 0)
	{
		GetWorldTimerManager().ClearTimer(RandomSpawnHandle);
		SpawnObject();
		MyMode->NumberOfRespawner--;
	}
	if(MyMode->IsStarted && MyMode->NumberOfRespawner == 0)
	{
		MyMode->IsStarted = false;
	}


}

void AEnemyRespawner::SpawnObject()
{
	UE_LOG(LogTemp, Display, TEXT("1"));
	if(TowerToSpawn)
	{
		GetWorld()->SpawnActor<ATower>(
		TowerToSpawn, 
		GetActorLocation(), 
		GetActorRotation()
		);
		TotalRespawnTank--;
	}
	if(TotalRespawnTank != 0)
	{

		float SpawnRate = FMath::RandRange(RandomRespawnMinRange, RandomRespawnMaxRange);
		GetWorldTimerManager().SetTimer(RandomSpawnHandle, this, &AEnemyRespawner::SpawnObject, SpawnRate , false);
	}
	
}
