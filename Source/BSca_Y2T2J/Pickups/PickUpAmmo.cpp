// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickUpAmmo.h"
#include "PlayerClass\PlayerCharacter.h"

void APickUpAmmo::SetAmmo(float ammo)
{
	AmmoAmount = 30;
	UE_LOG(LogTemp, Error, TEXT("Player Has picked up %d Amount of Ammo"), AmmoAmount);

}

void APickUpAmmo::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	SetAmmo(AmmoAmount);
	Destroy();

}

void APickUpAmmo::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
