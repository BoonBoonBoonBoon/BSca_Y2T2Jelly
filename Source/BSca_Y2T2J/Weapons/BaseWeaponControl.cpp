// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeaponControl.h"
#include "Components/SphereComponent.h"
#include "PlayerClass\PlayerCharacter.h"

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

void ABaseWeaponControl::Equip(APlayerCharacter* PlayerRefrence)
{
	if (PlayerRefrence) 
	{
		
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

	UE_LOG(LogTemp, Warning, TEXT("Weapon Pickup"));
	Destroy();
}

void ABaseWeaponControl::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

