// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeaponControl.h"
#include "Components/SphereComponent.h"
#include "PlayerClass\PlayerCharacter.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
ABaseWeaponControl::ABaseWeaponControl()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVol = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVol;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	Muzzle->SetupAttachment(SkeletalMesh);

	bIsWeapon = true;


	// Checks Rotation 
	bRotate = true;
	RotationRate = 90;

}

// Called when the game starts or when spawned
void ABaseWeaponControl::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseWeaponControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CollisionVol->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeaponControl::OnOverlapBegin);
	CollisionVol->OnComponentEndOverlap.AddDynamic(this, &ABaseWeaponControl::OnOverlapEnd);

	// Multiplies the Yaw rotation of item by every tick. 
	if (bRotate)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += RotationRate * DeltaTime;
		SetActorRotation(Rotation);
	}
}

void ABaseWeaponControl::Equip(APlayerCharacter* PlayerRefrence)
{
	if (PlayerRefrence)
	{
		if (bIsWeapon) {
			if (PlayerRefrence->CheckWeaponMeshIndex.Num() <= 2) {
				if (bIsRifle) {

					/**
					* So we make a new object pointer of type USkelmesh(this means we can access all of its functions and variables)
					* but the object has no infomation, other than it can look inside the class.
					* We assign the Rifle socket to the object. Now the object has infomation on what to do.
					*/
					const USkeletalMeshSocket* Socket = PlayerRefrence->GetMesh()->GetSocketByName("RifleHandSocket");
					// if the socket can get the infomation and it comes back true.
					
					// VVVVVVV The object (Socket)
					if (Socket) {
						/**
						* we then call a function from the UskeletalMesh class, We then put in two arguments.
						* so then we say the actor we want to attach is the Socket(This is because it holds the infomation about the socket mesh)
						* We then get the player mesh because we want to attach it to the body (The mesh in this instance is the body mesh subobject, i.e. the mesh in the viewport)
						*/
						Socket->AttachActor(this, PlayerRefrence->GetMesh());

						// Calls inline setter function 
						// This is the socket again because the IF is the parameter for the object 
						PlayerRefrence->SetEquippedWeapon(this);

						/* How we add an element to the array, AddUnique is what tells "this" to be added.
						* AddUnique is the same as add but it will not allow a replicate in the array and will Return it with a -1.*/
						PlayerRefrence->CheckWeaponMeshIndex.AddUnique(this);
						UE_LOG(LogTemp, Warning, TEXT("Weapon Id : %d"), PlayerRefrence->CheckWeaponMeshIndex.Num());
					}
				}
				else if (bIsShotGun) {
					const USkeletalMeshSocket* Socket = PlayerRefrence->GetMesh()->GetSocketByName("ShotGunHandSocket");
					if (Socket) {
						Socket->AttachActor(this, PlayerRefrence->GetMesh());
						PlayerRefrence->CheckWeaponMeshIndex.AddUnique(this);
						PlayerRefrence->SetEquippedWeapon(this);
						UE_LOG(LogTemp, Warning, TEXT("Weapon Id : %d"), PlayerRefrence->CheckWeaponMeshIndex.Num());
					}
				}
			}
		}
	}
}


void ABaseWeaponControl::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(OtherActor);
		if (PlayerRef) {
			Equip(PlayerRef);
		}
	}
	Destroy();
}

void ABaseWeaponControl::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

