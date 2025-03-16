// Fill out your copyright notice in the Description page of Project Settings.


#include "cppActor.h"

#include "K2Node_CustomEvent.h"

// Sets default values
AcppActor::AcppActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AcppActor::BeginPlay()
{
	Super::BeginPlay();
	
	Add(CurrentHealth, DamageFire);
	Subtract(CurrentHealth, DamageIce);
	Multiply(CurrentHealth, DamageFire);
	Divide(CurrentHealth, DamageIce);
}

// Called every frame
void AcppActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AcppActor::Add(float& value1, float value2)
{
	value1 = value1 + value2;
	UE_LOG(LogTemp, Warning, TEXT("El resultado de la suma de vida es: %f"), value1);
}

void AcppActor::Subtract(float& value1, float value2)
{
	value1 = value1 - value2;
	UE_LOG(LogTemp, Warning, TEXT("El resultado de la resta de vida es: %f"), value1);
}

void AcppActor::Multiply(float& value1, float value2)
{
	value1 = value1 * value2;
	UE_LOG(LogTemp, Warning, TEXT("El resultado de la multiplicacion de vida es: %f"), value1);
}

void AcppActor::Divide(float& value1, float value2)
{
	value1 = value1 / value2;
	UE_LOG(LogTemp, Warning, TEXT("El resultado de la division de vida es: %f"), value1);
}
