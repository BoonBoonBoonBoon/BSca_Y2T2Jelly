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
#include "Components/ArrowComponent.h"
#include "AI\BpLib_LootDrop.h"
#include "Weapons\BaseWeaponControl.h"
#include "Containers/Array.h"

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

	
	StaminaComp = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComp"));
	//  new
	while (!HealthComp) {
		HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	}


	/** Attached Subobjects */
	SpringArmComp->SetupAttachment(RootComponent);
	// Accessor needs to close off heirarchy. 
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->SetFieldOfView(85.f);

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
	MaxRifleInventoryAmmo = 90;
	// Sets the max amount of ammmo in inv
	RifleMaxAmmo = MaxRifleInventoryAmmo;

	RifleMagazineAmmo = 30;
	// Sets the max Amount of ammo in mag
	MaxDefaultRifleMagazineAmmo = RifleMagazineAmmo;

	//bHasAmmo = false; 
	bWantstoFire = true; 
	bIsFiring = false; 
	bIsReloading = false;
	bIsRifle = false;
	bIsShotgun = false;

	// Focused Movement values.
	ZoomWalkSpeed = 250.f;
	ZoomRunSpeed = 600.f;
	ZoomCrouchSpeed = 180.f;

	// Starting the weapon on a default value.
	WeaponIndex = 0;

	//CheckWeaponMeshIndex.Init()
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

	if (bWantstoFire && !bIsFiring)
	{
		if (!bIsReloading) {
			if (CheckWeaponMeshIndex[0]->GetName().Contains("Rifle")) {
				if (bHasRifleMagAmmo)
				{
					bIsFiring = true;
					UseAmmo();
					FireSingleProjectile();
				}
			}
			else if (CheckWeaponMeshIndex[0]->GetName().Contains("Shotgun")) {
				if (bHasShotgunMagAmmo) {
					bIsFiring = true;
					ShotgunUseAmmo();
					OnShotGunFire();
				}
			}
		}
	}
}

void APlayerCharacter::FireSingleProjectile()
{
	// checks if we have selected a bullet class 
	if (Projectileclass) {

		// Handle for spawn params.
		FActorSpawnParameters SpawnParams;
		// Collison Spawn params, Always spawns bullet no matter what
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// Set no fail so it has to spawn, spawning cannot fail
		SpawnParams.bNoFail = true;
		// Setting owner 
		SpawnParams.Owner = this;
		// the instigator is the actor that caused the damage, i.e. the person that shot the bullet.
		SpawnParams.Instigator = this;

		/* Transform for projectile, Not needed currently  */
		FTransform ProjectileSpawnTransForm;
		// Spawn projecitle infront of player
		ProjectileSpawnTransForm.SetLocation(GetActorForwardVector() * 00.f + GetActorLocation());
		// Actor rotation makes bullet face same rotation
		// Quaternion is another way of representing a rotation
		ProjectileSpawnTransForm.SetRotation(GetActorRotation().Quaternion());
		// set scale to make sure it doesnt warp
		ProjectileSpawnTransForm.SetScale3D(FVector(1.f));
		//GetWorld()->SpawnActor<AProjectileBase>(Projectileclass, ProjectileSpawnTransForm, SpawnParams);
		

		// Spawn Projectile from character view.
		FVector CamLoc = CameraComp->GetForwardVector();
		FRotator CamRot = GetActorRotation();
		GetActorEyesViewPoint(CamLoc, CamRot);
		GetWorld()->SpawnActor<AProjectileBase>(Projectileclass, CamLoc, CamRot, SpawnParams);
	}
}

/* Weapon Class Specific Shotgun functionality*/
void APlayerCharacter::OnShotGunFire()
{
	if (Projectileclass) {
		// Handle for spawn params.
		FActorSpawnParameters SpawnParams;
		// Collison Spawn params, Always spawns bullet no matter what
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// Set no fail so it has to spawn, spawning cannot fail
		SpawnParams.bNoFail = true;
		// Setting owner 
		SpawnParams.Owner = this;
		// the instigator is the actor that caused the damage, i.e. the person that shot the bullet.
		SpawnParams.Instigator = this;

		// Spawn Projectile from character view.
		FVector CamLoc = CameraComp->GetForwardVector();
		FRotator CamRot = GetActorRotation();
		GetActorEyesViewPoint(CamLoc, CamRot);
		// Top Right
		GetWorld()->SpawnActor<AProjectileBase>(Projectileclass, CamLoc, CamRot + FRotator(3.f, 5.f, 0.f), SpawnParams);
		// Top Middle
		GetWorld()->SpawnActor<AProjectileBase>(Projectileclass, CamLoc, CamRot + FRotator(5.f, -3.f, 0.f), SpawnParams);
		// Top Left
		GetWorld()->SpawnActor<AProjectileBase>(Projectileclass, CamLoc, CamRot + FRotator(3.f, -10.f, 0.f), SpawnParams);
		// Bottom Left
		GetWorld()->SpawnActor<AProjectileBase>(Projectileclass, CamLoc, CamRot + FRotator(-4.f, -12.f, 0.f), SpawnParams);
		// Bottom Right
		GetWorld()->SpawnActor<AProjectileBase>(Projectileclass, CamLoc, CamRot + FRotator(-4.f, 7.f, 0.f), SpawnParams);
		// Bottom Middle

		// Middle Right
		GetWorld()->SpawnActor<AProjectileBase>(Projectileclass, CamLoc, CamRot + FRotator(-1.f, -1.f, 0.f), SpawnParams);
		// Middle Top
		GetWorld()->SpawnActor<AProjectileBase>(Projectileclass, CamLoc, CamRot + FRotator(1.f, -3.f, 0.f), SpawnParams);
		// Middle Left
		GetWorld()->SpawnActor<AProjectileBase>(Projectileclass, CamLoc, CamRot + FRotator(-1.f, -6.f, 0.f), SpawnParams);
		// Middle Bottom
		GetWorld()->SpawnActor<AProjectileBase>(Projectileclass, CamLoc, CamRot + FRotator(0.f, 6.f, 0.f), SpawnParams);
	}
}


void APlayerCharacter::ManualShotgunReload() {


	if (bHasShotgunInvAmmo) {
		CallTracker = 3;
		bIsReloading = true;

		// Inrate is 1 since we want it count to go down every 1 second
		GetWorld()->GetTimerManager().SetTimer(FireDelayTimerHandle, this, &APlayerCharacter::ResetFire, 1, true);
	}

}

/* Player Input for Manual Reload */
void APlayerCharacter::ManualReload()
{
	
	if (bHasRifleInvAmmo) {
		CallTracker = 2;
		bIsReloading = true;

		// Inrate is 1 since we want it count to go down every 1 second
		GetWorld()->GetTimerManager().SetTimer(FireDelayTimerHandle, this, &APlayerCharacter::ResetFire, 1, true);
	}

}

// Timer Check Reloading and other functions.
void APlayerCharacter::ResetFire()
{

	if (bIsReloading) {
		// If reloadsec is equal or greater than call tracker will decrement
		CallTracker--;
		UE_LOG(LogTemp, Warning, TEXT("Timer Tick %d"), CallTracker);

		// Checks when the decrease reaches 0 reset timer 
		if (CallTracker == 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(FireDelayTimerHandle);
			// fmath min gets the difference of number say if you have 5 and 10 will return 5.
			int ReloadAmount = FMath::Min(MaxDefaultRifleMagazineAmmo - RifleMagazineAmmo, MaxRifleInventoryAmmo);
			RifleMagazineAmmo += ReloadAmount;
			MaxRifleInventoryAmmo -= ReloadAmount;
			bIsReloading = false;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Still Reloading"));
		}
	}
}

void APlayerCharacter::ResetShotgunFire()
{
	if (bIsReloading) {
		// If reloadsec is equal or greater than call tracker will decrement
		CallTracker--;
		UE_LOG(LogTemp, Warning, TEXT("Timer Tick %d"), CallTracker);

		// Checks when the decrease reaches 0 reset timer 
		if (CallTracker == 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(FireDelayTimerHandle);

			int ReloadShotAmt = FMath::Min(MaxDefaultShotgunMagazineAmmo - ShotgunMagazineAmmo, MaxShotgunInventoryAmmo);
			ShotgunMagazineAmmo += ReloadShotAmt;
			MaxShotgunInventoryAmmo -= ReloadShotAmt;
			bIsReloading = false;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Still Reloading"));
		}
	}
}

/* Calculates if Ammo can be inherited */
void APlayerCharacter::CheckAmmoPickup(int Ammo)
{
	//int AmmoInc;
	if (!bHasRifleMagAmmo && !bHasRifleInvAmmo) {

		RifleMagazineAmmo = UKismetMathLibrary::FClamp(RifleMagazineAmmo + Ammo, 0, MaxDefaultRifleMagazineAmmo);
		UE_LOG(LogTemp, Error, TEXT("Updated Magammo value: %d"), RifleMagazineAmmo);

	}
	else {
		MaxRifleInventoryAmmo = UKismetMathLibrary::FClamp(MaxRifleInventoryAmmo + Ammo, 0, RifleMaxAmmo);
		UE_LOG(LogTemp, Error, TEXT("Updated invammo value: %d"), MaxRifleInventoryAmmo);

	}
}

/* Changes Weapon Index */
void APlayerCharacter::SwitchWeapon()
{	
	// Check if the amount of elements is greater than zero
	if (CheckWeaponMeshIndex.Num() > 1)
	{
		ABaseWeaponControl* Temp = CheckWeaponMeshIndex[0];
		CheckWeaponMeshIndex[0] = CheckWeaponMeshIndex[1];
		CheckWeaponMeshIndex[1] = Temp;

		CheckWeaponMeshIndex[0]->DisableActor(false);
		CheckWeaponMeshIndex[1]->DisableActor(true);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Player has fewer than 2 weapons"));
	}
	
}

/* Subtracts Ammo*/
void APlayerCharacter::UseAmmo() 
{
	if (bHasRifleMagAmmo) {
		RifleMagazineAmmo = FMath::Clamp(RifleMagazineAmmo - RifleAmmoUse, 0.0f, MaxDefaultRifleMagazineAmmo);
		//UE_LOG(LogTemp, Error, TEXT("Ammo Use) Player Magazine Ammo is : %d"), MagazineAmmo)
	} 
}

void APlayerCharacter::ShotgunUseAmmo() {

	if (bHasShotgunMagAmmo) {
		ShotgunMagazineAmmo = FMath::Clamp(ShotgunMagazineAmmo - ShotgunAmmoUse, 0.0f, MaxDefaultShotgunMagazineAmmo);
		//UE_LOG(LogTemp, Error, TEXT("Ammo Use) Player Magazine Ammo is : %d"), MagazineAmmo)
	}

}


// Called When Character Is In Focus.
void APlayerCharacter::ZoomIn()
{
	bIsZoomedin = true;
	if (bIsZoomedin) {
		CameraComp->SetFieldOfView(70);
		CheckMovementBooleans(bIsWalking, bIsRunning, bIsCrouched, bIsFiring, bIsZoomedin == true);
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
		CameraComp->SetFieldOfView(90);
		CheckMovementBooleans(bIsWalking, bIsRunning, bIsCrouched, bIsFiring, bIsZoomedin == false);
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

	bHasRifleInvAmmo = MaxRifleInventoryAmmo > 0;
	bHasRifleMagAmmo = RifleMagazineAmmo > 0;
	if (!bHasRifleMagAmmo && bHasRifleInvAmmo && !bIsReloading) { // Update - Need to make sure the player isnt already ready
		//UE_LOG(LogTemp, Warning, TEXT("Player has no ammo in mag"));
		ManualReload();
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// added this new
	FTransform SpawnTransform;
	HealthComp = Cast<UHealthComponent>(AddComponentByClass(UHealthComponent::StaticClass(), false, SpawnTransform, false));

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


void APlayerCharacter::IncreaseHealth_Implementation(UHealthComponent* OtherActor)
{
}
void APlayerCharacter::OnStaminaUse_Implementation()
{
}
void APlayerCharacter::CameraSpin_Implementation()
{
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
	PlayerInputComponent->BindAction("ShotgunTest", IE_Pressed, this, &APlayerCharacter::OnShotGunFire);

	PlayerInputComponent->BindAction("ManualReload", IE_Pressed, this, &APlayerCharacter::ManualReload);
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &APlayerCharacter::SwitchWeapon);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &APlayerCharacter::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &APlayerCharacter::ZoomOut);
}

/*FHitResult FHit1;
FVector StartLoc1 = GetActorLocation() + FVector(40, 10, 10);
FVector ForwardVector1 = CameraComp->GetForwardVector() + FVector(0, 0.1f , 0.05f);
FVector EndLoc1((ForwardVector1 * 2500.f) + StartLoc1 );
FCollisionQueryParams CollisionParam1;

bool bHit1 = GetWorld()->LineTraceSingleByChannel(FHit1, StartLoc1, EndLoc1, ECC_Visibility, CollisionParam1);
if (bHit1) {
	DrawDebugLine(GetWorld(), StartLoc1, EndLoc1, FColor::Red, false, 2, 0, 3);
	DrawDebugSphere(GetWorld(), FHit1.ImpactPoint, 10, 4, FColor::Green, false, 4, 0, 3);
} */

//FHitResult FHit2;
//FVector StartLoc2 = GetActorLocation() + FVector(40, 10, 10);
//FVector ForwardVector2 = CameraComp->GetForwardVector();
//FVector EndLoc2((ForwardVector2 * 2500.f) + StartLoc2);
//FCollisionQueryParams CollisionParam2;
/*bool bHit2 = GetWorld()->LineTraceSingleByChannel(FHit2, StartLoc2, EndLoc2, ECC_Visibility, CollisionParam2);
if (bHit2) {
	DrawDebugLine(GetWorld(), StartLoc2, EndLoc2, FColor::Red, false, 2, 0, 3);
	DrawDebugSphere(GetWorld(), FHit2.ImpactPoint, 10, 4, FColor::Green, false, 4, 0, 3);
}*/

/*
*
*
* Add random offset + every unit the bullet travels
* fhit function
* Lookup Normalized vectors
*
* fhitresult - impact normal, turn the impact normal into spread.
*
FHitResult FHit;
FVector StartLoc = GetActorLocation() + FVector(40, 10, 10);
FVector ForwardVector = CameraComp->GetForwardVector();
FVector EndLoc((ForwardVector * 2500.f) + StartLoc);
FCollisionQueryParams CollisionParam;
bool bHit = GetWorld()->LineTraceSingleByChannel(FHit, StartLoc, EndLoc, ECC_Visibility, CollisionParam);

// get the distance from the tracestart to the location in the world space.
float dist = FHit.Distance;
// how many units it should spread.
float spread = 5.f;
// ?? maybe the offset?
float falloff = 100.f;

// if there is a hit, get world location where moving shape would end up against the impacted object
fvector newhit = fhit.location
	// find the normal of the hit location, so you know what axes to spread across
for (int shot = 0; shot < numofbullets; ++shot)
{
	// need to get the xyz because we cant just have it on one world axis
	float newhitoffsetY = (dist / falloff) * spread;
	float newhitoffsetZ = (dist / falloff) * spread;

	// RAND range generates a randim number inbetween a range (float InMin, float InMax)
	newhit.y += fmath::randrange(-newhitoffsety, newhitoffsety)
	newhit.z += fmath::randrange(-newhitoffsetz, newhitoffsetz)


	FHitResult FHit;
	FVector StartLoc = GetActorLocation() + FVector(40, 10, 10);
	// look up use links
	float direction = FMath::Normalize(newhit - StartLoc);
	FVector EndLoc(StartLoc + (direction * 2500.f));
	FCollisionQueryParams CollisionParam;
	bool bHit = GetWorld()->LineTraceSingleByChannel(FHit, StartLoc, EndLoc, ECC_Visibility, CollisionParam);
}

*/