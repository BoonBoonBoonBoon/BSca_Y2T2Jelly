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
	
	/** Movement Speed Variables **/

	// ... RunSpeed (TEMP) to hold temparate speed infomation. 
	float RunSpeedTemp;
	// ... WalkSpeed Avg (TEMP).
	float WalkSpeedAvg;
	// ... For the Max character speed.
	float RunSpeed; 
	// ... Character X Axis Turn Speed
	float TurnSpeed;
	
	/** Boolean Checks **/

	// ... Checks if Running.
	bool bIsRunning;
	// ... Checks if jumping.
	bool bIsJumping;

	/** Pickup Variables **/

	// ... For Characters Max speed with consumables. 
	float RunSpeedPickup;

	
	// ... Max Avg JumpHeight.
	float JumpHeight; 

	// ... CrouchSpeed.
	float CrouchSpeed;

	float CamClamp;



	// Creates StaticMeshComp.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeshComp")
		class UStaticMeshComponent* StaticMeshComp; 

	// Creates SpringArm (Used to attach to body/Cam)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArmComp")
		class USpringArmComponent* SpringArmComp; 

	// Creates Cam (Used for viewport distance)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraProp")
		class UCameraComponent* CameraComp; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CamManager")
		class APlayerController* PlayerControl;




private:

	void MoveVer(float Value);
	void MoveHor(float Value);

	void StartJump();

	void Run();
	void RunEnd();

	void StartCrouch();
	void EndCrouch();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CameraClamp();
	
private:




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
