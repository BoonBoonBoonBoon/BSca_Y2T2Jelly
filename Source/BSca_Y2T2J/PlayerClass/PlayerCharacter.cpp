// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerClass/PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework\Actor.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Health\HealthComponent.h"
#include "GameFramework\PlayerController.h"
#include "Camera\PlayerCameraManager.h"
#include "Stamina\StaminaComponent.h"
#include "Weapons/ProjectileBase.h"
#include "Pickups\PickUpAmmo.h"
#include "Weapons\BaseWeaponControl.h"

// Defines an Alias. Macro for reusablity.


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
	CameraComp->SetFieldOfView(90.f);

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

	GetCharacterMovement()->RotationRate = FRotator(0.f, -80.f, 0.f);

	WalkSpeedAvg = 500.f;
	RunSpeed = 1200.f;
	RunSpeedTemp = 1200.f;
	// XAxis turn Speed
	TurnSpeed = 0.5f;

	// Pickup Values
	RunSpeedPickup = 2000.f;
	JumpHeightPickup = 1000.f;

	// Sprinting jumpVelocity = (normaljump + or - "int");

	// Crouch Values
	CrouchSpeed = 250.f;

	// Jumping Values
	JumpHeight = 600.f;

	// Boolean Values
	bIsRunning = false;
	bIsJumping = false;
	bIsZoomedin = false;
	bIsWalking = false;

	// @See Idle
	bShouldRotate = false;
	RotationRate = 90.f;

	DefaultHealth = 100.f;
	Health = DefaultHealth;
	
	// Ammo & Firing.
	MaxDefaultAmmo = 90;
	MaxAmmo = MaxDefaultAmmo;

	DefaultMagazineAmmo = 90; 
	MagazineAmmo = DefaultMagazineAmmo;

	AmmoUse = 0;

	bHasAmmo = false; 
	bWantstoFire = false; 
	bIsFiring = false; 
	bIsRifle = false;
	bIsShotgun = false;

	// Focused Movement values.
	ZoomWalkSpeed = 250.f;
	ZoomRunSpeed = 600.f;
	ZoomCrouchSpeed = 180.f;
}

void APlayerCharacter::CheckBooleans(bool CheckWalk, bool CheckRun, bool CheckCrouch, bool CheckFire, bool CheckZoom) 
{
	if (bIsRunning == true && bIsZoomedin)
	{
		
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		
	}
	else if (bIsWalking == true && !bIsRunning && bIsZoomedin)
	{ 
		GetCharacterMovement()->MaxWalkSpeed = 250.f;
	}
	else if (bIsWalking && !bIsZoomedin && !bIsRunning && !bIsCrouched)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeedAvg;
	}
	else if (bIsCrouched == true && bIsZoomedin)
	{
		GetCharacterMovement()->MaxWalkSpeed = 180.f;
	}
	else 
	{
		return;
	}
}


									//// Movement


void APlayerCharacter::MoveVer(float Value)
{
	//if (bIsZoomedin) 
	
		//int WalkTemp = WalkSpeedAvg - 400;
		//GetCharacterMovement()->MaxWalkSpeed = WalkTemp;
		//AddMovementInput(GetActorForwardVector(), Value);
	CheckBooleans(true, NULL, NULL, NULL, bIsZoomedin);
	bIsWalking = true;
	if (bIsWalking) {
		AddMovementInput(GetActorForwardVector(), Value);
	}
		
	
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
		CheckBooleans(true, NULL, NULL, NULL, bIsZoomedin);

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

//https://www.dropbox.com/sh/po5yga1oke4n58j/AAC7kfAWBpg_Th8mLb7ZorPQa?dl=0


void APlayerCharacter::Run()
{
	/** Character changes to walkspeed when calling sprint method while its crouching
	* @see RunEnd method.
	*
	**/
	CheckBooleans(NULL, true, NULL, NULL, bIsZoomedin);

	bIsRunning = true;
	if (bIsRunning == true && bIsZoomedin == false) 
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else if (!bIsRunning)
	{
		RunEnd();
	}
}

void APlayerCharacter::RunEnd()
{
	bIsRunning = false;

	if (!bIsRunning && !bIsZoomedin) 
	{
		/**
		* MWS is AChar,
		* accessor needs to access class variables.
		* WSA Then used as temp data storage.
		*/
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeedAvg;
	}
	else 
	{
		return;
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

	CheckBooleans(NULL, NULL, true, NULL, bIsZoomedin);

	bIsCrouched = true;
	if (bIsZoomedin == true && bIsCrouched == false) 
	{
		GetCharacterMovement()->SetJumpAllowed(false);
		GetCapsuleComponent()->SetCapsuleHalfHeight(48.f);
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		APlayerCharacter::RunSpeed = CrouchSpeed;

	} else if (!bIsCrouched) 
	{
		EndCrouch();
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

void APlayerCharacter::OnBasicFire()
{
	/**
	To check if we can pickup anything to begin with we need to check if the max ammo has been hit
	So we need a max ammo var & if we are firing.
	and a if we want to fire var.
	*/

	bWantstoFire = true;
	bHasAmmo = MaxDefaultAmmo > 0;

	// Change with ammo pickup
	MaxAmmo = 90, MaxDefaultAmmo;
	UE_LOG(LogTemp, Warning, TEXT("Max Ammo is %d"), MaxAmmo);

	if (bWantstoFire == true && bIsFiring == false) 
	{
		if (bHasAmmo) 
		{
			MaxDefaultAmmo = MaxDefaultAmmo - AmmoUse;
			UE_LOG(LogTemp, Warning, TEXT("Player Ammo is %d"), );


			FHitResult FHit;
			FVector StartLoc = GetActorLocation() + FVector(40, 10, 10);
			FVector ForwardVector = CameraComp->GetForwardVector();
			FVector EndLoc((ForwardVector * 2500.f) + StartLoc);
			FCollisionQueryParams CollisionParam;
			bool bHit = GetWorld()->LineTraceSingleByChannel(FHit, StartLoc, EndLoc, ECC_Visibility, CollisionParam);

			if (bHit)
			{
				DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Red, false, 2, 0, 3);
				DrawDebugSphere(GetWorld(), FHit.ImpactPoint, 10, 4, FColor::Green, false, 4, 0, 3);
			}

		} else
			{
			UE_LOG(LogTemp, Error, TEXT("Player has ran out of ammo!"));
			return;
			}

	}
	// DONT FORGET MANUAL RELOAD

	//PickUpAmmo* AmmoPtr{};
	//AmmoPtr->OnOverlapBegin(GetCapsuleComponent(), );
	//UE_LOG(LogTemp, Error, TEXT("Player Now Has %d Amount of Ammo"), DefaultAmmo);
	//onactorhit destroy
	//FHitResult
}

void APlayerCharacter::OnShotGunFire()
{
}

void APlayerCharacter::ManualReload()
{

	UE_LOG(LogTemp, Error, TEXT("Reload"));

}

void APlayerCharacter::SwitchWeapon()
{
	UE_LOG(LogTemp, Error, TEXT("Switch"));
	//APlayerCharacter* Owner = this;
	//if (CheckWeaponIndex.Num() != 0) 
	//{
		
	//}

}


int APlayerCharacter::CalculateAmmo(int _ammoAmount)
{
	return 0;
}



void APlayerCharacter::ZoomIn() 
{
	bIsZoomedin = true;

	if (bIsZoomedin) {
		CameraComp->SetFieldOfView(70.f);
		CheckBooleans(bIsWalking, bIsRunning, bIsCrouched, bIsFiring, bIsZoomedin == true);
		UE_LOG(LogTemp, Error, TEXT("Zoomed In"));
	}
	else if (!bIsZoomedin) 
	{
		ZoomOut();
	}
}

void APlayerCharacter::ZoomOut() 
{
	bIsZoomedin = false;

	if (!bIsZoomedin) {
		CameraComp->SetFieldOfView(90.f);
		CheckBooleans(bIsWalking, bIsRunning, bIsCrouched, bIsFiring, bIsZoomedin == false);
		UE_LOG(LogTemp, Error, TEXT("Zoomed out"));
	}
	else 
	{
		return;
	}
}


						//// Tick & Begin Play


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckBooleans(bIsWalking, bIsRunning, bIsCrouched, bIsFiring, bIsZoomedin);
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
	//IncreaseHealthPtr->Regen = 20.f;
	//IncreaseHealthPtr->Health = FMath::Clamp(IncreaseHealthPtr->Health + IncreaseHealthPtr->Regen, 0.0f, IncreaseHealthPtr->DefaultHealth);
}
void APlayerCharacter::OnStaminaUse_Implementation()
{
}
void APlayerCharacter::CameraSpin_Implementation()
{
}

void APlayerCharacter::TimerFunction()
{
	// Timer Handle for Camera Rotation
	/*
	CallTracker--;
	if (CallTracker == 0)
	{GEngine->AddOnScreenDebugMessage(0, 150.f, FColor::Yellow, TEXT("Idle"));
	GetWorldTimerManager().ClearTimer(TimerHandle);}
	else{
		GEngine->AddOnScreenDebugMessage(0, 150.f, FColor::Yellow, TEXT("Idle Called but failed"));}
	*/
}


// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveVer", this, &APlayerCharacter::MoveVer);
	PlayerInputComponent->BindAxis("MoveHor", this, &APlayerCharacter::MoveHor);

	// ... Camera Yaw and Pitch Control

	/** Use of APawn since the method AddCYI is not a Construct of APlayerChar
	* @see LookPitch */
	PlayerInputComponent->BindAxis("Look Yaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Pitch", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::JumpEnd);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::RunEnd);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::EndCrouch);

	// bind fire events
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnBasicFire);
	PlayerInputComponent->BindAction("ManualReload", IE_Pressed, this, &APlayerCharacter::ManualReload);
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &APlayerCharacter::SwitchWeapon);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &APlayerCharacter::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &APlayerCharacter::ZoomOut);

	


	// Manual Reload 
	//Zoom in & zoom out
}

