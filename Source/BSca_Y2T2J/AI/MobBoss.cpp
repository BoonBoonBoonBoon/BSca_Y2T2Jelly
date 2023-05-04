// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MobBoss.h"

// Sets default values
AMobBoss::AMobBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

}

// Called to bind functionality to input
void AMobBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

