// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BSCA_Y2T2J_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void StaminaIncrease(float StaminaRed, AActor* StaminaCauser, AController* InstigatedBy, APlayerCharacter* Run);

	UFUNCTION()
	void StaminaDrain(float StaminaRed, AActor* StaminaCauser, AController* InstigatedBy, APlayerCharacter* Run);

	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Stamina")
		float defaultStamina;

	UPROPERTY(blueprintReadWrite, Category = "Stamina")
		float Stamina;
		
	UPROPERTY(BlueprintReadWrite)
		bool bIsMoving;

	
};
