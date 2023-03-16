// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpBase.generated.h"

UCLASS()
class BSCA_Y2T2J_API APickUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpBase();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
		class USkeletalMeshComponent* SkeletalMesh;


	// Creates Sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CollisionVol")
		class USphereComponent* CollisionVol;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float RotationRate;
	bool bRotate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Overlap Function
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// End Overlap Function
	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



};
