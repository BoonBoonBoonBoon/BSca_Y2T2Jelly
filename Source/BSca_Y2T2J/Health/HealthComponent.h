// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BSCA_Y2T2J_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

		class USphereComponent;

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Health")
		float DefaultHealth = 100;
	UPROPERTY(BlueprintReadOnly)
		float Health = 100;

	void AddHealth(float HealthChange);

	UFUNCTION() // ... Dynamic binding
		void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	//void IncreaseHealthWidget();
	//float Regen;
	
};
