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

	
	// Check to see current movement.
	bool bIsRunning;
	bool bIsJumping;
	bool bIsWalking;
	float TurnSpeed;

	// Movement Speed values, changes speed value.
	float RunSpeedPickup;
	float JumpHeightPickup; 
	float RunSpeedTemp;

	// Max amount of useable ammo, can be manipulated
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ammo")
	int MaxInventoryAmmo; 
	// Max Set ammo, Cannot be manipulated.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ammo")
	int MaxAmmo = 90; 

	// Useable ammo in the magazine.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ammo")
	int MagazineAmmo; 
	// Max Ammo in a magazine, 30.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ammo")
	int MaxDefaultMagazineAmmo = 30; 
	
	// Speed at which a weapon can fire.
	int fireRate;

	// Rifle Variables. 
	int RifleAmmoUse;
	int RifleAmmoDamage;

	// Check Current Ammo.
	bool bHasMagAmmo;
	bool bHasInvAmmo;
	//bool bHasAmmo; 


	// Check Status of Weapon.
	bool bIsReloading;
	bool bIsRifle;
	bool bIsShotgun;
	
	UFUNCTION(BlueprintCallable)
		virtual void Run();
		void RunEnd();


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
			void CheckMovementBooleans(bool CheckWalk, bool CheckRun, bool CheckCrouch, bool CheckFire, bool CheckZoom);

		UFUNCTION()
			void OnBasicFire(); // changed name from onriflefire, will be used as boilerplate as the rifle.
			void OnShotGunFire(); // shotgun params

		void CheckAmmoPickup(int Ammo);
		// Depletion of Ammo stock.
		void UseAmmo();

		// Reload Magazine (R).
		void ManualReload();
		// Reload Magazine (AU)
		//void AutomaticReload();

		// Switch Input (F).
		void SwitchWeapon();


		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
		class ABaseWeaponControl* EquippedWeapon;

		// Inline to return a Getter
		FORCEINLINE ABaseWeaponControl* GetEquippedWeapon() { return EquippedWeapon; }
		FORCEINLINE void SetEquippedWeapon(ABaseWeaponControl* WeaponRefrence) { EquippedWeapon = WeaponRefrence; }

		// Getter for Automatic Reload.
		FORCEINLINE void AutomaticReload() { ManualReload(); }

		/*Store in the array the pointers to the weapons we spawn with the name checkweapon*/
		UPROPERTY()
		TArray<ABaseWeaponControl*> CheckWeaponMeshIndex;

		// Focus Functions.
		void ZoomIn();
		void ZoomOut();

		// Check State of focus.
		bool bIsZoomedin;

		// Focus Speed Values.
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
	void ResetFire();

	// Handles Delay inbetween shots.
	FTimerHandle FireDelayTimerHandle;

	// Check Firing opportunity. 
	bool bWantstoFire;
	bool bIsFiring;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
