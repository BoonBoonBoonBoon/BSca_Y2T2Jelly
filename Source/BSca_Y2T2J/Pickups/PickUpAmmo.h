// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	int AmmoAmount;

	void SetAmmo(float ammo);

	// @see Override
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


};
