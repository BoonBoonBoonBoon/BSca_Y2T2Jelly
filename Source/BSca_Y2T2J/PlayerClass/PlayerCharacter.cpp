// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerClass/PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Health\HealthComponent.h"
#include "GameFramework\PlayerController.h"
#include "Camera\PlayerCameraManager.h"

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
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->bEnableCameraRotationLag = true;
	SpringArmComp->CameraLagSpeed = 4.f;
	SpringArmComp->CameraRotationLagSpeed = 4.f;

	
	/** Set Absoulte is opposite of Set relative
	* @see SetRelative
	* Works to ignore parent, means that it is absolute and will not change
	* otherwise specifed. */
	SpringArmComp->SetUsingAbsoluteRotation(false);
	SpringArmComp->TargetArmLength = 300.f;


	/**
	* if True. Rotatoes the pitch and yaw of the body
	* to match controller input
	* Pitch Transforms the body off the ground.*/
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Yaw Rotates the body.
	bUseControllerRotationYaw = true;


	// Maintains relative rotation to the parent
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetRelativeRotation(FRotator(0.f, 0.f, 100.f));
	SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	// Checks for collision
	SpringArmComp->bDoCollisionTest = true;

	// Checks if it should use the view/cont rotation of pawn
	// CameraComp->bUsePawnControlRotation = false;

	GetCharacterMovement()->RotationRate = FRotator(0.f, -80.f, 0.f);

	// Movement Values
	WalkSpeedAvg = 500.f;
	RunSpeed = 1200.f;
	// (Temp) 
	RunSpeedTemp = 1200.f;
	// XAxis turn Speed
	TurnSpeed = 0.5f;

	// Pickup Values
	RunSpeedPickup = 2000.f;
	JumpHeightPickup = 1000.f;


	// Crouch Values
	CrouchSpeed = 250.f;

	// Jumping Values
	JumpHeight = 600.f;

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
	GetCharacterMovement()->JumpZVelocity = JumpHeight;
	Jump();
		

}

void APlayerCharacter::JumpEnd()
{
	StopJumping();
	
}


void APlayerCharacter::Run()
{
	/** Character changes to walkspeed when calling sprint method while its crouching
	* @see RunEnd method.
	*
	**/

	bIsRunning = true;

	if (bIsRunning) {

		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	}
	else 
	{
		RunEnd();
	}

	

}

void APlayerCharacter::RunEnd()
{

	bIsRunning = false;

	if (!bIsRunning) {
		/**
		* MWS is AChar,
		* accessor needs to access class variables.
		* WSA Then used as temp data storage.
		*/
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeedAvg;
	}
	else {
		Run();
	}
}

void APlayerCharacter::StartCrouch()
{
	bIsCrouched = true;
	if (bIsCrouched) {
		GetCharacterMovement()->SetJumpAllowed(false);
		

		GetCapsuleComponent()->SetCapsuleHalfHeight(48.f);
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		APlayerCharacter::RunSpeed = CrouchSpeed;
	}
	
}

void APlayerCharacter::EndCrouch()
{

	bIsCrouched = false;

	if (!bIsCrouched) 
	{

		GetCharacterMovement()->MaxWalkSpeed = WalkSpeedAvg;
		APlayerCharacter::RunSpeed = RunSpeedTemp;
		APlayerCharacter::UnCrouch();
		GetCharacterMovement()->SetJumpAllowed(true);
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


	// DOCCUMENT FOR TOMORROW WEBSITE
	
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (PlayerController->PlayerCameraManager)
		{
			PlayerController->PlayerCameraManager->ViewPitchMin = -30.0;
			PlayerController->PlayerCameraManager->ViewPitchMax = 30.0;
		}
	} 
	
}

void APlayerCharacter::CameraClamp()
{
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
	PlayerInputComponent->BindAxis("Look Yaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Pitch", this, &APawn::AddControllerPitchInput);

	// ... Jumping Input Control
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::JumpEnd);

	// ... Running Input Control
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::RunEnd);

	// ... Crouch Input Control
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::EndCrouch);
	//PlayerInputComponent->BindAction("Crouch", IE_Repeat, this, &APlayerCharacter::StartCrouch);
	

}

