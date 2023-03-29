// Fill out your copyright notice in the Description page of Project Settings.


#include "Health/HealthComponent.h"
#include "Components\ProgressBar.h"
#include "Weapons\ProjectileBase.h"
#include "PlayerClass\PlayerCharacter.h"
#include "Pickups\PickUpBase.h" 

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//DefaultHealth = 100.f;
	//Health = DefaultHealth;



	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	/**
	* Set pointer of AActor Class,GetOwner is Method of actorcomp
	* if owner is true. Access OTAD event, use dynamic delegate to bind event and function
	* tell computer to look at TakeDamage function
	* @see takedamage
	*/

	// Get reference to owning actor,
	//AActor* Owner = GetOwner(); 
	//if (Owner)
//	{
	//	// Valid owner returned, whenever takedamage function called, add dynamic binding. 
	//	Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeDamage); 
//	}

	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//void UHealthComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
//{
	/**
	* @See Health
	* On TakeDamage Health is given the data,
	* FMath which is a struct for math problems.
	*
	* Use :: because in the class mathUtility FMath is a object like Clamp. Used to refrence object. 
	* 
	* @See Clamp
	* Uses Clamp method, it requires float and something to attach it to a double.
	*
	* Want to tell health to give its original health then minus the incoming damage float,
	* this tells health that we are getting the value of the health.
	* Clamp needs a minimum vaue and a maximum value
	* give it 0.0f, health wont go below 0.
	* give health back again since that will be the maximum value. 
	* 
	* Use if to check if the damage data has gone above 0.
	* if true it returns to functon TD,
	* Then minus it from health.
	*/


	//if (Damage <= 0)
	//{
	//	return;		// ... Checking if take damage. 
	//}

//	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);


//}

//Regen = 20.f;

	//Health = FMath::Clamp(Health + Regen, 0.0f, DefaultHealth);

//void UHealthComponent::IncreaseHealthWidget()
//
	

//}
