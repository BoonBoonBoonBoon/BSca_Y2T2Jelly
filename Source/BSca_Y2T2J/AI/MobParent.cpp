// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MobParent.h"
#include "Weapons\ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMobParent::AMobParent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MobDefaultHealth = 100.f;
	MobHealth = MobDefaultHealth;

	CollisionBody = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBody"));
	
}

void AMobParent::AddHealth(float HealthChange)
{

	//// takes input of a value from 10 to 20
	//float HealthInc;
	//// should return the value of the health increase
	//HealthInc = UKismetMathLibrary::FClamp(HealthChange, 0.f, MobDefaultHealth);
	//UE_LOG(LogTemp, Warning, TEXT("Mob Health before Being Added: %f"), MobHealth);
	//UE_LOG(LogTemp, Warning, TEXT("Mob Healthinc value: %f"), HealthInc)

	//MobHealth = FMath::Clamp(MobHealth + HealthInc, 0.f, MobDefaultHealth);

	//UE_LOG(LogTemp, Error, TEXT("Updated health value: %f"), MobHealth)

}

void AMobParent::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

	if (Damage <= 0)
	{
		return;		// ... Checking if take damage. 
	}
	MobHealth = FMath::Clamp(MobHealth - Damage, 0.0f, MobDefaultHealth);
	UE_LOG(LogTemp, Error, TEXT("Mob Health After Being Damaged : % f"), MobHealth);

}

// Called when the game starts or when spawned
void AMobParent::BeginPlay()
{
	Super::BeginPlay();
	
	// Get reference to owning actor,
	
	// Valid owner returned, whenever takedamage function called, add dynamic binding. 
	//CollisionBody->OnComponentBeginOverlap.AddDynamic(this, &AMobParent::OnTakeDamage);

}

// Called every frame
void AMobParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OnTakeDamage(NULL, NULL, NULL, NULL, NULL);
}

// Called to bind functionality to input
void AMobParent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}

