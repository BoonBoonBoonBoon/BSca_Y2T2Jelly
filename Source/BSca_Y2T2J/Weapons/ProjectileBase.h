// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class BSCA_Y2T2J_API AProjectileBase : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
		//class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StaticMesh")
		class UStaticMeshComponent* StaticMesh;

	// Creates Sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CollisionBody")
		class USphereComponent* CollisionBody;

		AProjectileBase* ProjectilePtr;

		UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
			class UHealthComponent* HealthComp2;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
			class UProjectileMovementComponent* ProjectileMovement;

		void FireInDirection(const FVector& ShootDir);

		void RifleDamageModi(AActor* AIActor);

		void ShotgunDamageModi(AActor* AIActor);

		int RifleDamage;
		int ShotgunDamage;

		//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerRef")
		//	APlayerCharacter* PlayerRefrence;

		//// Used to call Weapon Boolean Checks
		//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponRef")
		//	ABaseWeaponControl* WeaponRefrence;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) ;



};
