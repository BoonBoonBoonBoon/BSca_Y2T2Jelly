// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MobBoss.h"
#include "Weapons\ProjectileBase.h"
#include "PlayerClass\PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI\BpLib_LootDrop.h"

// Sets default values
AMobBoss::AMobBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BossDefaultHealth = 3000.f;
	BossHealth = BossDefaultHealth;

	bIsDead = false;
}

void AMobBoss::AddHealth(float HealthChange)
{
}

void AMobBoss::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{


	if (Damage <= 0)
	{
		return;		// ... Checking if take damage. 
	}
	BossHealth = FMath::Clamp(BossHealth - Damage, 0.0f, BossDefaultHealth);
	UE_LOG(LogTemp, Error, TEXT("Boss health : % f"), BossHealth);

	ReceiveAnyDamage(Damage, NULL, NULL, NULL);

}

void AMobBoss::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AMobBoss::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts or when spawned
void AMobBoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMobBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnTakeDamage(NULL, NULL, NULL, NULL, NULL);

	if (BossHealth == 0) {
		bIsDead = true;
		DropLoot(bIsDead);
		Destroy();
	}
}

// Called to bind functionality to input
void AMobBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

