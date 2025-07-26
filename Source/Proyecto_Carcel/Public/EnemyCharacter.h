// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "INTERFACES/DamageableInterface.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"
class UStaticMeshComponent;

UCLASS()
class PROYECTO_CARCEL_API AEnemyCharacter : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//	Health variables
	float MaxHealth = 100;
	float CurrentHealth = 100;
	//	Damage interface function
	virtual void ApplyDamage_Implementation(float DamageAmount) override;

	void SetExclamationColor (FLinearColor LinearColor);

	
	//	Static mesh component for representing player seen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Target")
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Enemy|Target")
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Target")
	float VisionRadius = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Target")
	int32 NumberOfLines = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Target")
	float MaxAngle = 45;

	FVector StartOffset = FVector(0, 0, 88);
};