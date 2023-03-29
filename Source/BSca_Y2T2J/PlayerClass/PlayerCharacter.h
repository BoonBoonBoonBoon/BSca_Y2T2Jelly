// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/ProjectileBase.h"
#include "Health\HealthComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class BSCA_Y2T2J_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

		class UCharacterMovementComponent;


public:
	// Sets default values for this character's properties
	APlayerCharacter();


	// Creates StaticMeshComp.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeshComp")
		class UStaticMeshComponent* StaticMeshComp;

	// Creates SpringArm (Used to attach to body/Cam)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArmComp")
		class USpringArmComponent* SpringArmComp;

	// Creates Cam (Used for viewport distance)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraProp")
		class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaminaComponent* StaminaComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Health")
		float DefaultHealth = 100;
	UPROPERTY(BlueprintReadOnly)
		float Health = 100;

	UFUNCTION() // ... Dynamic binding
		void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	
	float TurnSpeed;
	bool bIsRunning;
	bool bIsJumping;
	// Pickups 
	float RunSpeedPickup;
	float JumpHeightPickup;
	// (TEMP) 
	float RunSpeedTemp;


	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float WalkSpeedAvg;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float RunSpeed; 
	UPROPERTY(BlueprintReadWrite, Category = "CameraSpin")
		bool bShouldRotate;
		float RotationRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float JumpHeight; 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CrouchSpeed;


	UFUNCTION(BlueprintCallable)
		virtual void Run();
		void RunEnd();
	UFUNCTION(BlueprintCallable)
		void Idle();
	UFUNCTION()
		void InteractPressed();
		float bullets;


	// Widget Health Increase.
	UFUNCTION(BlueprintNativeEvent, Category = "HealthIncrease")
		void IncreaseHealth();
		void IncreaseHealth_Implementation();


	// Widget Stamina Decrease.
	UFUNCTION(BlueprintNativeEvent, Category = "OnStaminaUse")
		void OnStaminaUse();
		void OnStaminaUse_Implementation();


	// Camera Spin.
	UFUNCTION(BlueprintNativeEvent, Category = "CameraSpin")
		void CameraSpin();
		void CameraSpin_Implementation();


	// Spawn Projectile.
	UPROPERTY(EditAnywhere, Category = "TSubClass")
		TSubclassOf<AProjectileBase> Projectileptr;


private:

	void MoveVer(float Value);
	void MoveHor(float Value);



	UFUNCTION(BlueprintCallable)
	void StartJump();
	void JumpEnd();


	void StartCrouch();
	void EndCrouch();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Camera Time Handle , Max Timer loops. Number of times it will be tracking.
	int32 CallTracker = 3;
	
private:




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Timer Function
	void TimerFunction();

	// Timer Handle (For Control). 
	FTimerHandle TimerHandle;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
