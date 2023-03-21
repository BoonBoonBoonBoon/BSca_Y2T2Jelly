// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UHealthComponent* HealthComp;





	// RunSpeed (TEMP) to hold temparate speed infomation. 
	float RunSpeedTemp;

	// WalkSpeed Avg (TEMP).
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float WalkSpeedAvg;

	// For the Max character speed.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float RunSpeed; 

	// Character X Axis Turn Speed
	float TurnSpeed;

	// Checks if Running.
	bool bIsRunning;

	// Checks if jumping.
	bool bIsJumping;

	// For Characters Max speed with consumables. 
	float RunSpeedPickup;
	float JumpHeightPickup;


	// Max Avg JumpHeight.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float JumpHeight; 

	// CrouchSpeed.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CrouchSpeed;



	/** 
	* Can be used both in C and Bp 
	* BlueprintNativeEvent you WILL NEED to define _Implementation function
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "OnStaminaUse")
		void OnStaminaUse();
		void OnStaminaUse_Implementation();


	// Camera Spin
	UFUNCTION(BlueprintNativeEvent, Category = "CameraSpin")
		void CameraSpin();
		void CameraSpin_Implementation();

	UPROPERTY(BlueprintReadWrite, Category = "CameraSpin")
		bool bShouldRotate;
		float RotationRate;

	UFUNCTION(BlueprintCallable)
		void Idle();


	UFUNCTION(BlueprintCallable)
		virtual void Run();
		void RunEnd();


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
