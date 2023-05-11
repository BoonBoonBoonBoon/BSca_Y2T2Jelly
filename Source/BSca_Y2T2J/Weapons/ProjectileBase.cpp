// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/ProjectileBase.h"
#include "Health/HealthComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PlayerClass\PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework\Pawn.h"
#include "Weapons\BaseWeaponControl.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AI\MobParent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI\MobBoss.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBody = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBody"));
	RootComponent = CollisionBody;

	HealthComp2 = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp2"));
	//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	//StaticMesh->SetupAttachment(GetRootComponent());

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3500.f;
	ProjectileMovement->MaxSpeed = 4000.f;

	MobPtr = Cast<AMobParent>(UGameplayStatics::GetActorOfClass(GetWorld(), 0));

}

// NULL FUNCTION
void AProjectileBase::FireInDirection(const FVector& ShootDir)
{
	// i dont know
	//ProjectileMovement->Velocity = ShootDir * ProjectileMovement->InitialSpeed;
	UE_LOG(LogTemp, Warning, TEXT("Valid Projectile"));

}

void AProjectileBase::RifleDamageModi(AActor* AIActor)
{
	AMobParent* MobActor = Cast<AMobParent>(AIActor);
	if (MobActor) {

		int RandDamageMod = UKismetMathLibrary::RandomFloatInRange(10, 35);
		UE_LOG(LogTemp, Warning, TEXT("RandomRifleDamage : %d"), RandDamageMod);
		MobActor->OnTakeDamage(NULL, RandDamageMod, NULL, NULL, NULL);
	}
	else {
		AMobBoss* BossActor = Cast<AMobBoss>(AIActor);

		int RandBossDamageMod = UKismetMathLibrary::RandomFloatInRange(10, 35);
		UE_LOG(LogTemp, Warning, TEXT("RandomRifleDamage : %d"), RandBossDamageMod);
		BossActor->OnTakeDamage(NULL, RandBossDamageMod, NULL, NULL, NULL);

	}
	// if (bIsRifle){
	//	int RandDamageMod  UKismetMathLibrary::RandomIntergerInRange(15, 30);
	//	UE_LOG(LogTemp, Warning, TEXT("RandomRifleDamage : %d"), RandDamageMod);
	// 
	// 
	// Dont even need this thinking about it now.
	//	RifleDamage = UKismetMathLibrary::FClamp(RandDamage, 0.f, 30);
	//


}

void AProjectileBase::ShotgunDamageModi(AActor* AIActor)
{



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

	if (OtherActor) {
		AMobParent* Mob = Cast<AMobParent>(OtherActor);
		AMobBoss* Boss = Cast<AMobBoss>(OtherActor);
		if (Mob) {
			RifleDamageModi(Mob);
			UE_LOG(LogTemp, Warning, TEXT("AI Hit"));
			Destroy();
		}
		else if(Boss){
			RifleDamageModi(Boss);
			UE_LOG(LogTemp, Warning, TEXT("AI Hit"));
			Destroy();
		}
	}
	
	

}

void AProjectileBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

