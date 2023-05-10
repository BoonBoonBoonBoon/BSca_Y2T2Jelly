//// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MobParent.h"
#include "Weapons\ProjectileBase.h"
#include "PlayerClass\PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI\BpLib_LootDrop.h"

// Sets default values
AMobParent::AMobParent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MobDefaultHealth = 125.f;
	MobHealth = MobDefaultHealth;

	CollisionBody = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBody"));
	//TriggerComp = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Volume"));
	
	bIsDead = false;
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


	ReceiveAnyDamage(Damage, NULL, NULL, NULL);
}


// Called when the game starts or when spawned
void AMobParent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMobParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OnTakeDamage(NULL, NULL, NULL, NULL, NULL);

	if (MobHealth == 0) {
		bIsDead = true;
		DropLoot(bIsDead);
		Destroy();
	}
}

// Called to bind functionality to input
void AMobParent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}

void AMobParent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*if (TriggerComp) {
		UE_LOG(LogTemp, Warning, TEXT("Trigger"));
		APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(OtherActor);
		AProjectileBase* ProjectileRef = Cast<AProjectileBase>(OtherActor);
			if(PlayerRef) {
				//UpdateTrigger(PlayerRefrence);
			}
			else if (ProjectileRef) {
				return;
			}
	}
	*/
}

void AMobParent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}



