// Fill out your copyright notice in the Description page of Project Settings.


#include "Health/HealthComponent.h"
#include "Components\ProgressBar.h"
#include "Weapons\ProjectileBase.h"
#include "PlayerClass\PlayerCharacter.h"
#include "Pickups\PickUpBase.h" 
#include "Pickups\PickupHealth.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DefaultHealth = 100.f;
	Health = DefaultHealth;

	HealthRegenTracker = 0;
	CombatTracker = 0;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	bInCombat = false;

	// Get reference to owning actor,
	AActor* Owner = GetOwner(); 
	if (Owner)
	{
		// Valid owner returned, whenever takedamage function called, add dynamic binding. 
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeDamage); 
	}
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	OnTakeDamage(NULL, NULL, NULL, NULL, NULL);

	//HealthRegeneration();

}

// Loop increase health till value reached.
void UHealthComponent::HealthRegeneration()
{
	if (!bInCombat) {
		float HealthInc = 0.25f;
		Health += HealthInc;

		if (Health >= 75.f) {
			UE_LOG(LogTemp, Warning, TEXT("Health Fully Healed"));
			GetWorld()->GetTimerManager().ClearTimer(HealthRegenHandle);
		}
	}
	else {
		return;
	}
}

// Loop to determine out of combat timer.
void UHealthComponent::CheckCombat()
{
	if (bInCombat) {
		CombatTracker--;
		if (CombatTracker == 0) {
			bInCombat = false;
			// Clears Combat timer I.E. This
			GetWorld()->GetTimerManager().ClearTimer(CombatHandle);

			// Sets the Health regen timer
			GetWorld()->GetTimerManager().SetTimer(HealthRegenHandle, this, &UHealthComponent::HealthRegeneration, 0.05, true);
		}
	}
	else {
		return;
	}
}

// Health Increase & Pickup.
void UHealthComponent::AddHealth(float HealthChange)
{	
	// takes input of a value from 10 to 20
	float HealthInc;
	// should return the value of the health increase
	HealthInc = UKismetMathLibrary::FClamp(HealthChange, 0.f, DefaultHealth);
	//UE_LOG(LogTemp, Warning, TEXT("Health before Being Added: %f"), Health);
	//UE_LOG(LogTemp, Warning, TEXT("Healthinc value: %f"), HealthInc)

	Health = FMath::Clamp(Health + HealthInc, 0.f, DefaultHealth);
	
	UHealthComponent* HealthComponent = this;
	if (HealthComponent) {
		APlayerCharacter* PlayerPtr = Cast<APlayerCharacter>(HealthComponent);
		PlayerPtr->IncreaseHealth_Implementation(HealthComponent);
	}
	UE_LOG(LogTemp, Error, TEXT("Updated health value: %f"), Health)

}

// Recieve Damage.
void UHealthComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	DamageToPlayer = Damage;
	if (DamageToPlayer <= 0)
	{
		return;		// ... Checking if take damage. 
	}
	else if (DamageToPlayer > 0) {
	
		Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
		UE_LOG(LogTemp, Error, TEXT("Health After Being Damaged: %f"), Health);

		bInCombat = true;
		CombatTracker = 10;
		GetWorld()->GetTimerManager().SetTimer(CombatHandle, this, &UHealthComponent::CheckCombat, 1, true);
	}
}


