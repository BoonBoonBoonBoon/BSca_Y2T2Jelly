// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerClass\PlayerCharacter.h"
#include "Health\HealthComponent.h"
#include "Pickups/PickUpBase.h"
#include "PickupHealth.generated.h"

/**
 * 
 */
UCLASS()
class BSCA_Y2T2J_API APickupHealth : public APickUpBase
{
	GENERATED_BODY()

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		//Access Player Character
	APlayerCharacter* PlayerChar;

		//Access HealthComponent
	UHealthComponent* HealthComp;


		// @see Override
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


};
