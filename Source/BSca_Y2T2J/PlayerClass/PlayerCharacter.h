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

	// Spawn Projectile.
	UPROPERTY(EditAnywhere, Category = "TSubClass")
		TSubclassOf<AProjectileBase> Projectileptr;


	float TurnSpeed;
	bool bIsRunning;
	bool bIsJumping;
	bool bIsWalking;
	// Pickups 
	float RunSpeedPickup;
	float JumpHeightPickup;
	// (TEMP) 
	float RunSpeedTemp;

	// Firing Variables
	int MaxDefaultAmmo; // Max amount of useable ammo, can be manipulated.
	int MaxAmmo; // Max Set ammo, Cannot be manipulated.
	int DefaultMagazineAmmo; // Max Ammo in a magazine, 30.
	int MagazineAmmo; // Useable ammo in the magazine.
	int AmmoUse; // Bullets used, for rifle would be 1 per call, but shotgun would be 3 - 5.
	bool bHasAmmo; // checks ammo
	bool bWantstoFire; // checks if it CAN fire
	bool bIsFiring; // checks if it IS firing
	bool bIsRifle;
	bool bIsShotgun;
	
	UFUNCTION(BlueprintCallable)
		virtual void Run();
		void RunEnd();

	UFUNCTION(BlueprintCallable)
		void Idle();

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

		UFUNCTION() // ... Dynamic binding
			void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

		void CheckBooleans(bool CheckWalk, bool CheckRun, bool CheckCrouch, bool CheckFire, bool CheckZoom);

		// Weapons


		UFUNCTION()
			void OnBasicFire(); // changed name from onriflefire, will be used as boilerplate as the rifle.
			void OnShotGunFire(); // shotgun params

		void ManualReload();
		void SwitchWeapon();


		//void EquippedWeapon();

		class ABaseWeaponControl* EquippedWeapon;

		// Calculate the ammo in the magazine.
		int CalculateAmmo(int _ammoAmount);

		// Weapon Array
		UPROPERTY()
		TArray<ABaseWeaponControl*> CheckWeaponIndex;

		//FORCEINLINE

		// zoom types
		void ZoomIn();
		void ZoomOut();

		bool bIsZoomedin;
		int ZoomWalkSpeed;
		int ZoomRunSpeed;
		int ZoomCrouchSpeed;

		


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
