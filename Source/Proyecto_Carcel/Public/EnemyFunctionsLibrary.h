// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EnemyFunctionsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROYECTO_CARCEL_API UEnemyFunctionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//	Library function that loops linetraces within an max/min angle
	static FHitResult ConeTraceByChannel(const UWorld* WorldContext, const FVector &StartLocation, const int32 NumberOfLines, const float Angle, const float ConeLenght, const FVector &Forward, const FVector &Normal);
};

