// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponControl.generated.h"

UCLASS()
class BSCA_Y2T2J_API ABaseWeaponControl : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeaponControl();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
		class USkeletalMeshComponent* SkeletalMesh;


	// Creates Sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CollisionVol")
		class USphereComponent* CollisionVol;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Equip(class APlayerCharacter* PlayerRefrence);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperty")
		bool bIsWeapon = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperty")
		bool bIsRifle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponProperty")
		bool bIsShotGun;

	bool bRotate;
	int RotationRate;
	
	
	// Muzzle mesh for the weapons. 
	USceneComponent* Muzzle;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// End Overlap Function
	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
