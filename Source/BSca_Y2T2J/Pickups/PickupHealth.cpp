// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupHealth.h"
#include "Pickups/PickUpBase.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void APickupHealth::BeginPlay()
{
	Super::BeginPlay();
	/**
	* Casting here gives the pickup class access to everything inside the player class.
	* a common communication method where you take a 
	* refrence to an actor and try to convert it to 
	* a different class
	*/
	PlayerChar = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void APickupHealth::CheckHealth(UHealthComponent* HealthComp)
{
	// Randomize a float value 
	float RandomHealth = UKismetMathLibrary::RandomFloatInRange(10, 20);

	UE_LOG(LogTemp, Warning, TEXT("CheckHealthFunc: %f"), RandomHealth);
	// give arg random value to health to calculate
	HealthComp->AddHealth(RandomHealth);
}

void APickupHealth::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == PlayerChar) {


		//UE_LOG(LogTemp, Warning, TEXT("Health %s"), *PlayerChar->HealthComp->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("Default Health %f"), PlayerChar->HealthComp->DefaultHealth);
		

		if (PlayerChar->HealthComp->Health < PlayerChar->HealthComp->DefaultHealth) {
			CheckHealth(PlayerChar->HealthComp);
			Destroy();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Cannot pickup"));
		}
	}
}

void APickupHealth::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
