// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickUpBase.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Pickups\PickupHealth.h"
#include "Health\HealthComponent.h"

// Sets default values
APickUpBase::APickUpBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	/** Create a new component
	* Connect it to the root.
	* This makes a "Collision" Sphere, where anything that hits the sphere
	* will be calculated under the collison volume. */
	CollisionVol = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVol;

	/** New Component, Attaches it to Root.
	* Gives this the body.
	*/
	SkeletalMesh =CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh")); 
	SkeletalMesh->SetupAttachment(GetRootComponent());
	
	
	// Checks Rotation 
	bRotate = true;
	RotationRate = 70;

}

// Called when the game starts or when spawned
void APickUpBase::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void APickUpBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** Checks everysecond if Collision volume has been hit
	* if it has, it accesses PrimComp (USphere is Child of UPrim)
	* calls the overlap function
	* AddDynamic (Delegates) binds a function and an event (overlap event)
	* Calls for the Function.*/


	/**
	* Checks if the Rotate bool is true
	* Return RootComp location to Rotation var
	* Get the Y axis of FR roation plus Rotation Rate
	* Multiply it by Deltatime (Ticks)
	* Set Rotation just updates the new location every second
	*/

	// Delegates Event to Function
	CollisionVol->OnComponentBeginOverlap.AddDynamic(this, &APickUpBase::OnOverlapBegin);
	CollisionVol->OnComponentEndOverlap.AddDynamic(this, &APickUpBase::OnOverlapEnd);

	// Multiplies the Yaw rotation of item by every tick. 
	if (bRotate)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += RotationRate * DeltaTime;
		SetActorRotation(Rotation);
	}
}

void APickUpBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

		UE_LOG(LogTemp, Warning, TEXT("Base Pickup"));
		Destroy();
	

}

void APickUpBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

