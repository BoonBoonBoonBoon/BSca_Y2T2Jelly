// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerClass/PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sets Capsule Comp Size 
	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.f);

	/** @See PlayerInputComp for Yaw&Pitch */

	/** Create Components Sub objects, Used for attaching. */
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	/** Attached Subobjects */
	SpringArmComp->SetupAttachment(RootComponent);
	// Accessor needs to close off heirarchy. 
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	// Camera Lag Speed
	SpringArmComp->bEnableCameraLag = 50.f;
	SpringArmComp->bEnableCameraRotationLag = 50.f;
	
	/** Set Absoulte is opposite of Set relative
	* @see SetRelative
	* Works to ignore parent, means that it is absolute and will not change
	* otherwise specifed. 
	*/
	SpringArmComp->SetUsingAbsoluteRotation(true);
	SpringArmComp->TargetArmLength = 500.f;

	// Movement Values
	WalkSpeedAvg = 500.f;
	RunSpeed = 1200.f;
	// (Temp) 
	RunSpeedTemp = 1200.f;
	// XAxis turn Speed
	TurnSpeed;

	// Pickup Values
	RunSpeedPickup = 2000.f;

	// Crouch Values
	CrouchSpeed = 250.f;

	// Jumping Values
	JumpHeight = 250.f;

	// Boolean Values
	bIsRunning = false; 
	bIsJumping = false;
	

}

void APlayerCharacter::MoveVer(float Value)
{

	AddMovementInput(GetActorForwardVector(), Value);

}

void APlayerCharacter::MoveHor(float Value)
{

	/**
	* If Ctrl & Value are not NULL
	* Var Rotation of Type FRotator Gets Cntrl rotation method.
	* Var YawRtn type FRotator assigned X & Z Axis = 0.
	* Y Axis gets rotations YAxis
	* Var Dir of type FVector assigned @see FRotationMatrix
	* @FRM constructs matrix represents pure rotation, no translation or scale
	*/

	if ((Controller != nullptr) && (Value != 0.0f))
	{

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);

	}


}

void APlayerCharacter::StartJump()
{
	/**
	* Once method starts on the stack checks to see if bJuming is true
	* On true, Call AChar Jump method, Change height for higher of lower jump status. 
	* @see JumpZ
	* If false return AChar SJ Method 
	*/
	bIsJumping = true;

	if (bIsJumping) 
	{
		Jump();
		GetCharacterMovement()->JumpZVelocity = 600.f;
	}
	else 
	{
		StopJumping();
	}

}


void APlayerCharacter::Run()
{

	bIsRunning = true;

	// Access MWS AChar var, assign RunSpeed data.
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed; 

	if (bIsCrouched) {

		APlayerCharacter::StartCrouch();
		RunEnd();

	}
	
}

void APlayerCharacter::RunEnd()
{

	bIsRunning = false;

	/**
	* MWS is AChar,
	* accessor needs to access class variables.
	* WSA Then used as temp data storage. 
	*/
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeedAvg;
}

void APlayerCharacter::StartCrouch()
{

	GetCapsuleComponent()->SetCapsuleHalfHeight(48.f);

	bIsCrouched = true;
	bIsRunning = false;

	if (bIsCrouched) 
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		APlayerCharacter::RunSpeed = CrouchSpeed;
	}
	else if (bIsCrouched && !bIsRunning)
	{
		
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		APlayerCharacter::RunSpeed = CrouchSpeed;
		APlayerCharacter::WalkSpeedAvg = CrouchSpeed;

	}
	else if (!bIsCrouched) 
	{
		APlayerCharacter::EndCrouch();
	}
}

void APlayerCharacter::EndCrouch()
{

	//bIsCrouched = false;

	if (!bIsCrouched) 
	{

		GetCharacterMovement()->MaxWalkSpeed = WalkSpeedAvg;
		APlayerCharacter::RunSpeed = RunSpeedTemp;
		APlayerCharacter::UnCrouch();

	}
	else if (bIsCrouched)
	{

		bIsRunning = false;

	}

}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ... Move Vertical and Horizontally Input Control
	PlayerInputComponent->BindAxis("MoveVer", this, &APlayerCharacter::MoveVer);
	PlayerInputComponent->BindAxis("MoveHor", this, &APlayerCharacter::MoveHor);

	// ... Camera Yaw and Pitch Control

	/** Use of APawn since the method AddCYI is not a Construct of APlayerChar
	* Apawn used to reference the premade method
	* @see LookPitch */
	PlayerInputComponent->BindAxis("LookYaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookPitch", this, &APawn::AddControllerPitchInput);

	// ... Jumping Input Control
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StartJump);

	// ... Running Input Control
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::RunEnd);

	// ... Crouch Input Control
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::EndCrouch);
	//PlayerInputComponent->BindAction("Crouch", IE_Repeat, this, &APlayerCharacter::StartCrouch);
	

}

