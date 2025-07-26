// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFunctionsLibrary.h"
#include "Kismet/KismetMathLibrary.h"

FHitResult UEnemyFunctionsLibrary::ConeTraceByChannel(const UWorld* WorldContext, const FVector &StartLocation, const int32 NumberOfLines, const float Angle, const float ConeLenght, const FVector &Forward, const FVector &Normal)
{
	FHitResult HitObject;
	
	for (int32 i = 0; i < NumberOfLines; i++)
	{
		float IncrementAngle  = Angle / NumberOfLines * i;

		FVector EndLocation = StartLocation + (UKismetMathLibrary::RotateAngleAxis(Forward, IncrementAngle, Normal) * ConeLenght);

		FCollisionQueryParams Params;
		UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContext);

		
		World->LineTraceSingleByChannel(HitObject, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, Params);
		DrawDebugLine(World, HitObject.TraceStart, HitObject.TraceEnd, HitObject.bBlockingHit ? FColor::Blue : FColor::Yellow, false, 0.1f, 0, 1.0f);

		if (HitObject.bBlockingHit)
		{
			return HitObject;
		}
		else
		{
			EndLocation = StartLocation + (UKismetMathLibrary::RotateAngleAxis(Forward, (IncrementAngle * -1), Normal) * ConeLenght);
			
			World->LineTraceSingleByChannel(HitObject, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, Params);
			DrawDebugLine(World, HitObject.TraceStart, HitObject.TraceEnd, HitObject.bBlockingHit ? FColor::Blue : FColor::Yellow, false, 0.1f, 0, 1.0f);

			if (HitObject.bBlockingHit)
			{
				return HitObject;
			}
		}
	}
	
	return HitObject;
}

