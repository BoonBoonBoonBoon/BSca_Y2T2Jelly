// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "BpLib_LootDrop.generated.h"


// Custom struct that ties an object class to an integer to determine drop rate 

USTRUCT(BlueprintType)
struct FLoot_Box
{
    GENERATED_USTRUCT_BODY()

public:

    // TArray of the Object class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Array")
        TSubclassOf <AActor> ObjectClass;

    // Drop rate for the objects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Arrat", meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
        int32 DropRate;

};

UCLASS()
class BSCA_Y2T2J_API UBpLib_LootDrop : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    /* Input "Loot drop" struct array and their respective "drop rate" ( 50 = 50% chance of being selected),
    output will be the randomly selected class. The boolean output is for error handling to ensure the total "drop rate" = 100
    Node will not function unless the total is exactly 100, negative values will be treated as their positive counterparts */

    UFUNCTION(BlueprintCallable, meta = (Category = "Object Array", Keywords = "Loot, Drop, Random, Loot Box, Chance"))
        static void RandomDropRate(TArray <FLoot_Box> Loot_Classes, TSubclassOf <AActor>& Drop_Item, bool& Drop_Rate_Equal_100);

};
