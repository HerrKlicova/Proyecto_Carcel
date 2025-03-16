// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "cppActor.generated.h"

UCLASS()
class PROYECTO_CARCEL_API AcppActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcppActor();

	//VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Componentes")
	float MaxHealth = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Componentes")
	float CurrentHealth = 100;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Componentes")
	float DamageIce = 30;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Componentes")
	float DamageFire = 15;

	//FUNCIONES
	void Add(float& value1, float value2);
	void Subtract(float& value1, float value2);
	void Multiply(float& value1, float value2);
	void Divide(float& value1, float value2);


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
