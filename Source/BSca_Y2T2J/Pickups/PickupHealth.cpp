// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupHealth.h"
#include "Pickups/PickUpBase.h"
#include "Health/HealthComponent.h"
//Used to access character
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


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

		//HealthComp;
}

void APickupHealth::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	


	UE_LOG(LogTemp, Warning, TEXT("Health Pickup"));
	Destroy();
	
}

void APickupHealth::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
