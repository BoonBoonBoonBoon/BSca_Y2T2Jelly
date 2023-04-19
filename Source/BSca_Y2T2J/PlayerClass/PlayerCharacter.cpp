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

	/* Camera Boom Values */
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

	// Ammo & Firing.
	RifleAmmoUse = 1;
	MaxInventoryAmmo = 90;
	// Sets the max amount of ammmo in inv
	MaxAmmo = MaxInventoryAmmo;

	MagazineAmmo = 30;
	// Sets the max Amount of ammo in mag
	MaxDefaultMagazineAmmo = MagazineAmmo; 

	bHasAmmo = false; 
	bWantstoFire = true; 
	bIsFiring = false; 
	bIsReloading = false;
	bIsRifle = false;
	bIsShotgun = false;

	// Focused Movement values.
	ZoomWalkSpeed = 250.f;
	ZoomRunSpeed = 600.f;
	ZoomCrouchSpeed = 180.f;

	/* Reserve 3 spaces before array has to resize */
	CheckWeaponMeshIndex.Reserve(3);
	
}


void APlayerCharacter::CheckMovementBooleans(bool CheckWalk, bool CheckRun, bool CheckCrouch, bool CheckFire, bool CheckZoom) 
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
	CheckMovementBooleans(true, NULL, NULL, NULL, bIsZoomedin);
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
		CheckMovementBooleans(true, NULL, NULL, NULL, bIsZoomedin);

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::Run()
{
	CheckMovementBooleans(NULL, true, NULL, NULL, bIsZoomedin);
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
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeedAvg;
	}
	else 
	{
		return;
	}
}



void APlayerCharacter::StartJump()
{
	GetCharacterMovement()->JumpZVelocity = JumpHeight;
	Jump();
}

void APlayerCharacter::JumpEnd()
{
	StopJumping();
}


void APlayerCharacter::StartCrouch()
{
	CheckMovementBooleans(NULL, NULL, true, NULL, bIsZoomedin);
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


/* Weapon Class Specific Rifle functionality*/
void APlayerCharacter::OnBasicFire()
{
	bWantstoFire = true;
	bIsFiring = false;
	bHasInvAmmo = MaxInventoryAmmo > 0;
	bHasMagAmmo = MagazineAmmo > 0;
	bHasAmmo = MaxInventoryAmmo > 0, MagazineAmmo > 0;

	if (bWantstoFire && !bIsFiring ) 
	{
		if (bHasMagAmmo) 
		{
			bIsFiring = true;
			UseAmmo();
		
			FHitResult FHit;
			FVector StartLoc = GetActorLocation() + FVector(40, 10, 10);
			FVector ForwardVector = CameraComp->GetForwardVector();
			FVector EndLoc((ForwardVector * 2500.f) + StartLoc);
			FCollisionQueryParams CollisionParam;
			bool bHit = GetWorld()->LineTraceSingleByChannel(FHit, StartLoc, EndLoc, ECC_Visibility, CollisionParam);

			if (bHit) {
				DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Red, false, 2, 0, 3);
				DrawDebugSphere(GetWorld(), FHit.ImpactPoint, 10, 4, FColor::Green, false, 4, 0, 3);
			}
		} else {
			UE_LOG(LogTemp, Error, TEXT("OnFire) Player has ran out of Ammo for the Magazine!"));
			return;
		}
	}
}

/* Weapon Class Specific Shotgun functionality*/
void APlayerCharacter::OnShotGunFire()
{
}

/* Player Input for Manual Reload */
void APlayerCharacter::ManualReload()
{
		// keep in mind bool wantto and canfire
	if (bHasInvAmmo) {
			int MagAmmoStorage = 0;
			UE_LOG(LogTemp, Error, TEXT("Reloading"));
			bIsReloading = true;
			GetWorld()->GetTimerManager().SetTimer(FireDelayTimerHandle, this, &APlayerCharacter::ResetFire, 3, false, 3);
			if (bIsReloading) {
				// Store Magazine ammo value (Int Between 0 to 30) 30
				MagAmmoStorage = MagazineAmmo;
				// reduce ammo to zero (Empty The Mag) 0
				MagazineAmmo = 0;
				// Store inventory ammo value (Int Between 0 to 90) 90
				int InventoryAmmoStorage = MaxInventoryAmmo;
				// NewMagazine Cannot Go higher or lower than 30 and zero. - MagContainer cannot go below 0 and higher than 30, add Whatever value inventory ammo has inbetween 0/30
				int NewMagazine = FMath::Clamp(MagazineAmmo + MaxInventoryAmmo, 0, MaxDefaultMagazineAmmo);
				// Taking away a whole mag from Ammo inventory (Subtract 30 from the DefAmmo Int (0 to 90))
				MaxInventoryAmmo = FMath::Clamp(MaxInventoryAmmo - NewMagazine, 0, MaxAmmo);
				// Refill the Magazine to full (30 - 0)
				MagazineAmmo = NewMagazine;
				// Redistribute the original amount of ammo from magazineAmmo
				MaxInventoryAmmo = MaxInventoryAmmo + MagAmmoStorage;

			} else {
			UE_LOG(LogTemp, Error, TEXT("Reload Ammo) Player does not hold anymore Invetory Ammo! "));
			}
	} else {
		UE_LOG(LogTemp, Error, TEXT("Still Reloading"));
	}
}

/* Calculates if Ammo can be inherited */
void APlayerCharacter::CheckAmmoPickup(int Ammo)
{
	MaxInventoryAmmo = FMath::Clamp(MaxInventoryAmmo + Ammo, 0, MaxAmmo);
	UE_LOG(LogTemp, Error, TEXT("Player Picked up %d bullets"), Ammo);
}

/* Changes Weapon Index */
void APlayerCharacter::SwitchWeapon()
{
	/* Only fire off if we have something in the array
	so if its greater than zero*/
	if(CheckWeaponMeshIndex.Num() > 0){
		//UE_LOG(LogTemp, Error, TEXT("Switch"));
	}

}

/* Subtracts Ammo*/
void APlayerCharacter::UseAmmo() 
{
	if (bHasMagAmmo) {
		MagazineAmmo = FMath::Clamp(MagazineAmmo - RifleAmmoUse, 0.0f, MaxDefaultMagazineAmmo);
		//UE_LOG(LogTemp, Error, TEXT("Ammo Use) Player Magazine Ammo is : %d"), MagazineAmmo)
	} 
}


// Called When Character Is In Focus.
void APlayerCharacter::ZoomIn()
{
	bIsZoomedin = true;

	if (bIsZoomedin) {
		CameraComp->SetFieldOfView(70.f);
		CheckMovementBooleans(bIsWalking, bIsRunning, bIsCrouched, bIsFiring, bIsZoomedin == true);
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
		CheckMovementBooleans(bIsWalking, bIsRunning, bIsCrouched, bIsFiring, bIsZoomedin == false);
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
	CheckMovementBooleans(bIsWalking, bIsRunning, bIsCrouched, bIsFiring, bIsZoomedin);
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Yellow, TEXT("Ignore Stamina and Health Pickups WIP"));
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


					//// Events


void APlayerCharacter::IncreaseHealth_Implementation()
{
}
void APlayerCharacter::OnStaminaUse_Implementation()
{
}
void APlayerCharacter::CameraSpin_Implementation()
{
}

void APlayerCharacter::ResetFire()
{
	if (bIsReloading) {
		bIsReloading = false;
		GetWorld()->GetTimerManager().ClearTimer(FireDelayTimerHandle);
	}
	else { 
		return;
	}
}


// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveVer", this, &APlayerCharacter::MoveVer);
	PlayerInputComponent->BindAxis("MoveHor", this, &APlayerCharacter::MoveHor);

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

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnBasicFire);
	PlayerInputComponent->BindAction("ManualReload", IE_Pressed, this, &APlayerCharacter::ManualReload);
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &APlayerCharacter::SwitchWeapon);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &APlayerCharacter::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &APlayerCharacter::ZoomOut);
}

