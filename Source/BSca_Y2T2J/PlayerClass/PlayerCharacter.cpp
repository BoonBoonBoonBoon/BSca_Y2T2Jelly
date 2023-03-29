// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerClass/PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework\Actor.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Health\HealthComponent.h"
#include "GameFramework\PlayerController.h"
#include "Camera\PlayerCameraManager.h"
#include "Stamina\StaminaComponent.h"
#include "Weapons/ProjectileBase.h"

// Defines an Alias. Macro for reusablity.
#define UsePrintString(String) GEngine->AddOnScreenDebugMessage(-1.f, 150.f, FColor::White, String );


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

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	StaminaComp = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComp"));


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

	// @See Idle
	bShouldRotate = false;
	RotationRate = 90.f;
	
	DefaultHealth = 100.f;
	Health = DefaultHealth;


	
}


									//// Movement


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


void APlayerCharacter::Run()
{
	/** Character changes to walkspeed when calling sprint method while its crouching
	* @see RunEnd method.
	*
	**/

	//https://www.dropbox.com/sh/po5yga1oke4n58j/AAC7kfAWBpg_Th8mLb7ZorPQa?dl=0

	bIsRunning = true;

	if (bIsRunning) {

		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		//StaminaComp->StaminaDrain();
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


						//// Ineractions


void APlayerCharacter::Idle()
{
	// @See TimerFunction
	/** SetTimer First Set TimerHandle.
	* this. Context Actor for location.
	* Grab timer method.
	* inRate how often it shows on screen
	* Checks if loop
	* firs delay
	*/
	//GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::TimerFunction, 2.0f, true, 1.f);
}

void APlayerCharacter::InteractPressed()
{
	GEngine->AddOnScreenDebugMessage(1, 10, FColor::White, "Shooting");

	/*
	// Defines it in current level. 
	UWorld* WLRD = GetWorld();
	// Sets temp location to spawn actor.
	FVector Location = GetActorLocation() + FVector(60.f, 0.f, 20.f);
	FRotator Rotation = GetActorRotation();
	WLRD->SpawnActor(Projectileptr, &Location, &Rotation);
	*/

	// Line trace for debug.

	FHitResult FHit;
	FVector StartLoc = GetActorLocation();
	FVector ForwardVector = CameraComp->GetForwardVector();
	FVector EndLoc((ForwardVector * 2500.f) + StartLoc);
	FCollisionQueryParams CollisionParam;
	GetWorld()->LineTraceSingleByChannel(FHit, StartLoc, EndLoc, ECC_Visibility, CollisionParam);
	DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Red,false, 2, 0, 3);

	


	//onactorhit destroy
	//FHitResult
}

void APlayerCharacter::TimerFunction()
{

	// Timer Handle for Camera Rotation

	/*
	CallTracker--;

	if (CallTracker == 0)
	{

		GEngine->AddOnScreenDebugMessage(0, 150.f, FColor::Yellow, TEXT("Idle"));
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 150.f, FColor::Yellow, TEXT("Idle Called but failed"));

	}
	*/
}

	
						//// Tick & Begin Play


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Yellow, TEXT("Ignore Stamina and Health Pickups WIP"));
	

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


	// Get reference to owning actor.
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		// Delegate Function 
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::OnTakeDamage);
	}



	
}


					//// Events
void APlayerCharacter::OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	
	if (Damage <= 0)
	{
		return;		
	}
	// Reduces Health by damage
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);


}

void APlayerCharacter::IncreaseHealth_Implementation()
{
	//UE_LOG(LogTemp, Warning, TEXT("Health Pickup"));

	//IncreaseHealthPtr->Regen = 20.f;
	//IncreaseHealthPtr->Health = FMath::Clamp(IncreaseHealthPtr->Health + IncreaseHealthPtr->Regen, 0.0f, IncreaseHealthPtr->DefaultHealth);
}



void APlayerCharacter::OnStaminaUse_Implementation()
{
}

void APlayerCharacter::CameraSpin_Implementation()
{

	// Get Help for Cam Spins
	// @See Idle 

	//AActor* MyOwner = GetOwner();
	//SpringArmComp = Cast<USpringArmComponent>(MyOwner);
	//FRotator CameraRotation;
	//CameraRotation = SpringArmComp->GetComponentRotation();

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

	// Fire
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::InteractPressed);

}

