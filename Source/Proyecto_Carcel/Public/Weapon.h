// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INTERFACES/InteractuableInterface.h"
#include "PLAYER CHARACTER/COMPONENTS/ItemTypes.h"
#include "Weapon.generated.h"

class UBoxComponent;
UCLASS()
class PROYECTO_CARCEL_API AWeapon : public AActor, public IInteractuableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	//	Weapon Mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FText InteractionMessage;
	
	//	Data for the inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FItemData WeaponItemData;
	
	//	Returns the data for the inventory
	UFUNCTION(BlueprintCallable)
	virtual FItemData GetItemData_Implementation();

	UFUNCTION(BlueprintCallable)
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual FText GetInteractionText_Implementation() override {return InteractionMessage; };
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
