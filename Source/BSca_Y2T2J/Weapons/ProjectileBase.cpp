// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileBase.h"
//#include "Health/HealthComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
//#include "PlayerClass\PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework\Pawn.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBody = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBody"));
	RootComponent = CollisionBody;

	

	/** New Component, Attaches it to Root.
	* Gives this the body.
	*/
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

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

// Doesnt Work 
	/*
	ProjTakeDamage = Cast<UHealthComponent>(OtherActor);
	UHealthComponent::TakeDamage;

	GetFirstPlayerController()->GetCharacter());

	UWorld* WorldRef = GetWorld();
	UHealthComponent* HealthRef = Cast<UHealthComponent>(WorldRef->GetFirstPlayerController()->GetCharacter());
	if(HealthRef)
	{


		HealthRef->TakeDamage();

	}
	*/

void AProjectileBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	/*
	APlayerCharacter* PlayerChar = Cast<UHealthComponent>(OtherActor);
	if (Cast<UHealthComponent>(OtherActor)) {
		UE_LOG(LogTemp, Warning, TEXT("Take Damage"));
	};
 --------
	if (Damage <= 0)
	{
		return;		// ... Checking if take damage. 
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	*/
	
	//Owner = Cast<UHealthComponent>(&TakeDamagePtr)

	//OnTakeDamage(20.f, FDamageEvent(), NULL, NULL);

	UE_LOG(LogTemp, Warning, TEXT("Damage"));
	Destroy();

}

void AProjectileBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

