// Fill out your copyright notice in the Description page of Project Settings.


#include "Stamina/StaminaComponent.h"
#include "PlayerClass\PlayerCharacter.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	Stamina = 100.f;
	defaultStamina = Stamina;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();


	/** Get Owner Ref Pointer so we can call RunFunction
	* Get Actor Owner pointer so we can get owner
	* GetOwner Works as a way to not self inflict - 
	* we are using GetOwner(), then it’s because the class we are writing in is an ActorComponent, a component to be added to an actor, not the actor itself. 
	*/

	// https://www.dropbox.com/sh/po5yga1oke4n58j/AAC7kfAWBpg_Th8mLb7ZorPQa?dl=0
	
	// See paper - Timer Manager - GetTimerManager() - Onstamina Event
}


// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStaminaComponent::StaminaDrain(float StaminaRed, AActor* StaminaCauser, AController* InstigatedBy, APlayerCharacter* Run)
{

	/**
	* Clamps Reduced Stamina Value to Original Stamina value.
	* @See TakeDamage - HealthComp
	*/ 
	if (StaminaRed <= 0) 
	{
		return;
	}
	


	Stamina = FMath::Clamp(defaultStamina - StaminaRed, 0.0f, Stamina);


}

