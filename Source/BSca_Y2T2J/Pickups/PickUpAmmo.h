// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerClass\PlayerCharacter.h"
#include "Pickups/PickUpBase.h"
#include "PickUpAmmo.generated.h"

/**
 * 
 */
UCLASS()
class BSCA_Y2T2J_API APickUpAmmo : public APickUpBase
{
	GENERATED_BODY()
	

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo Type")
		bool bIsRifleAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo Type")
		bool bIShotgunAmmo;

	int RifleAmmoAmount;

	APlayerCharacter* PlayerChar = nullptr;

	void SetAmmo(APlayerCharacter* Actor);

	// @see Override
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


};
