// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TookTanksGameMode.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	// Bind the callback function to the OnTakeAnyDamage delegate
	// Delegate doesn't exist on the HealthComponent, it exists on Actor specifically, so we GetOwner to get Actor
	// will response once a Damage Event gets generated
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
	ToonTanksGameMode = Cast<ATookTanksGameMode>(UGameplayStatics::GetGameMode(this));

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// callback function 
void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if(Damage <= 0.f) return;
	Health -= Damage;
	if(Health <= 0 && ToonTanksGameMode)
	{
		ToonTanksGameMode->ActorDied(DamagedActor);
	}


}

