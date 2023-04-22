// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickUpAmmo.h"
#include "PlayerClass\PlayerCharacter.h"


void APickUpAmmo::SetAmmo()
{


}

void APickUpAmmo::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		APlayerCharacter* PlayerRefAmmo = Cast<APlayerCharacter>(OtherActor);
		if (PlayerRefAmmo) {
			
			if (bIsRifleAmmo) {
				if (PlayerRefAmmo->MaxInventoryAmmo < 90 && PlayerRefAmmo->MagazineAmmo > 0) {
					PlayerRefAmmo->CheckAmmoPickup(RifleAmmoAmount);

					Destroy();
				}
				else if (PlayerRefAmmo->MaxInventoryAmmo >= 90) {
					UE_LOG(LogTemp, Error, TEXT("Inventory Full!"));
				}
				else if (PlayerRefAmmo->MagazineAmmo == 0) {
					PlayerRefAmmo->CheckAmmoPickup(RifleAmmoAmount);
				}
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Wrong Ammo"));
			}
		}
	}
	
}


void APickUpAmmo::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	
}
