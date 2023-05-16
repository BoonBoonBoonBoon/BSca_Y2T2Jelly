// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickUpAmmo.h"
#include "PlayerClass\PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"


void APickUpAmmo::SetAmmo(APlayerCharacter* Actor)
{
	int RandomAmmo = UKismetMathLibrary::RandomIntegerInRange(10, 20);
	UE_LOG(LogTemp, Warning, TEXT("SetAmmoFunc: %d"), RandomAmmo);
	Actor->CheckAmmoPickup(RandomAmmo);

}

void APickUpAmmo::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		APlayerCharacter* PlayerRefAmmo = Cast<APlayerCharacter>(OtherActor);
		if (PlayerRefAmmo) {
			
			if (bIsRifleAmmo) {
				if (PlayerRefAmmo->MaxRifleInventoryAmmo < 90 && PlayerRefAmmo->RifleMagazineAmmo > 0) {
					//PlayerRefAmmo->CheckAmmoPickup(RifleAmmoAmount);
					SetAmmo(PlayerRefAmmo);
					Destroy();
				}
				else if (PlayerRefAmmo->MaxRifleInventoryAmmo >= 90) {
					UE_LOG(LogTemp, Error, TEXT("Inventory Full!"));
				}
				else if (PlayerRefAmmo->RifleMagazineAmmo == 0) {
					//PlayerRefAmmo->CheckAmmoPickup(RifleAmmoAmount);
					SetAmmo(PlayerRefAmmo);
					Destroy();
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
