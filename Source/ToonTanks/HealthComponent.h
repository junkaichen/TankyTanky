// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health = 0.f;


private:
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;
	

	// Callback function for OnTakeAnyDamage (must have specific parameter list, function name can be DIY)
	/*Parameter:
	Actor taking the damage
	Damage amount
	Damage Types (Fire, poision, etc)
	An instigator is the controller responsible for the damage
	Actor causing the damage*/
	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser);

	class ATookTanksGameMode* ToonTanksGameMode;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
