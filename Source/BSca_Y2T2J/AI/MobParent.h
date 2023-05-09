// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MobParent.generated.h"

UCLASS()
class BSCA_Y2T2J_API AMobParent : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMobParent();

	

	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Health")
		float MobDefaultHealth;
	UPROPERTY(BlueprintReadOnly)
		float MobHealth;

	// Creates Sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CollisionBody")
		class USphereComponent* CollisionBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		class USphereComponent* TriggerComp;

	UFUNCTION(BlueprintImplementableEvent, Category = "Drops")
		void DropLoot(bool ActorDeath);
	

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IsDead?")
			bool bIsDead;
	UFUNCTION()
		void AddHealth(float HealthChange);

	UFUNCTION()
		void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintNativeEvent, Category = "HealthUpdate")
		void UpdateHealth(float HealthUpdate);
		void UpdateHealth_Implementation(float HealthUpdate);


		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
			class APlayerCharacter* PlayerRefrence;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
