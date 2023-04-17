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

}

//void ABaseWeaponControl::CheckWeapon(bool Rifle, bool ShotGun) {
//}

void ABaseWeaponControl::Equip(APlayerCharacter* PlayerRefrence)
{
	if (PlayerRefrence) 
	{
		if (bIsWeapon)
		{
			if (bIsRifle) {
				const USkeletalMeshSocket* Socket = PlayerRefrence->GetMesh()->GetSocketByName("RifleHandSocket");
				if (Socket) {
					Socket->AttachActor(this, PlayerRefrence->GetMesh());
					PlayerRefrence->SetEquippedWeapon(this);
					UE_LOG(LogTemp, Warning, TEXT("Rifle Equip"));
				}
			} else if (bIsShotGun){
				const USkeletalMeshSocket* Socket = PlayerRefrence->GetMesh()->GetSocketByName("ShotGunHandSocket");
				if (Socket) {
					Socket->AttachActor(this, PlayerRefrence->GetMesh());
					PlayerRefrence->SetEquippedWeapon(this);
					UE_LOG(LogTemp, Warning, TEXT("Shotgun Equip"));
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
