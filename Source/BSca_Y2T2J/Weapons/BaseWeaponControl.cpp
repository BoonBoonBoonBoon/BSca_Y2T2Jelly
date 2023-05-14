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
	RiflePickedup = false;
	ShotGunPickedup = false;
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
		if (bIsRifle && !RiflePickedup) {
			FRotator Rotation = GetActorRotation();
			Rotation.Yaw += RotationRate * DeltaTime;
			SetActorRotation(Rotation);
		}
		else if (bIsShotGun && !ShotGunPickedup) {
			FRotator Rotation = GetActorRotation();
			Rotation.Yaw += RotationRate * DeltaTime;
			SetActorRotation(Rotation);
		}
	}
}

void ABaseWeaponControl::Equip(APlayerCharacter* PlayerRefrence)
{
	if (PlayerRefrence)
	{
		if (bIsWeapon) {
			if (PlayerRefrence->CheckWeaponMeshIndex.Num() <= 2) {
				if (bIsRifle) {
					const USkeletalMeshSocket* Socket = PlayerRefrence->GetMesh()->GetSocketByName("RifleHandSocket");
					if (Socket) {
						RiflePickedup = true;

						Socket->AttachActor(this, PlayerRefrence->GetMesh());
						PlayerRefrence->SetEquippedWeapon(this);
						PlayerRefrence->CheckWeaponMeshIndex.AddUnique(this);
						PlayerRefrence->SwitchWeapon();

						UE_LOG(LogTemp, Warning, TEXT("Weapon Id : %d"), PlayerRefrence->CheckWeaponMeshIndex.Num());
					}
				} else if (bIsShotGun) {
					const USkeletalMeshSocket* Socket = PlayerRefrence->GetMesh()->GetSocketByName("ShotGunHandSocket");
					if (Socket) {
						ShotGunPickedup = true;
						
						Socket->AttachActor(this, PlayerRefrence->GetMesh());
						PlayerRefrence->CheckWeaponMeshIndex.AddUnique(this);
						PlayerRefrence->SetEquippedWeapon(this);
						PlayerRefrence->SwitchWeapon();

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
			UE_LOG(LogTemp, Warning, TEXT("Player ref good"));
			Equip(PlayerRef);
			CollisionVol->OnComponentBeginOverlap.Clear();
		}
		
	}
}

void ABaseWeaponControl::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

