// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickUpAmmo.h"
#include "PlayerClass\PlayerCharacter.h"


void APickUpAmmo::SetAmmo()
{
	//APlayerCharacter* PlayerRef = nullptr;
	//PlayerRef->MaxInventoryAmmo += AmmoAmount;

}

void APickUpAmmo::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(OtherActor);
		if (PlayerRef) {
			PlayerRef->MaxInventoryAmmo;
			if (PlayerRef->MaxInventoryAmmo < 90) {
				PlayerRef->CheckAmmoPickup(AmmoAmount);
				Destroy();
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Cant pickup ammo!"));
			}
		}
	}
	
}


void APickUpAmmo::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	
}
