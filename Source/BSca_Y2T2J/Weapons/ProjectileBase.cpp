// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileBase.h"
#include "Health/HealthComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PlayerClass\PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework\Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBody = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBody"));
	RootComponent = CollisionBody;

	HealthComp2 = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp2"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	//StaticMesh->SetupAttachment(GetRootComponent());

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3500.f;
	ProjectileMovement->MaxSpeed = 4000.f;

	/** New Component, Attaches it to Root.
	* Gives this the body.
	*/
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

}

void AProjectileBase::FireInDirection(const FVector& ShootDir)
{
	// i dont know
	//ProjectileMovement->Velocity = ShootDir * ProjectileMovement->InitialSpeed;
	UE_LOG(LogTemp, Warning, TEXT("Valid Projectile"));

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// Reduces Size 
	CollisionBody->InitSphereRadius(20.f);

}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CollisionBody->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlapBegin);
	CollisionBody->OnComponentEndOverlap.AddDynamic(this, &AProjectileBase::OnOverlapEnd);

}


void AProjectileBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	

	UE_LOG(LogTemp, Warning, TEXT("Damage"));
	Destroy();

}

void AProjectileBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

