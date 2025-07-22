// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INTERFACES/InteractuableInterface.h"
#include "PLAYER CHARACTER/COMPONENTS/ItemTypes.h"
#include "FireWeaponInterface.h"
#include "Weapon.generated.h"

class UBoxComponent;
UCLASS()
class PROYECTO_CARCEL_API AWeapon : public AActor, public IInteractuableInterface, public IFireWeaponInterface
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

	UPROPERTY(EditAnywhere, Category = "Interact")
	TEnumAsByte<ECollisionChannel> FireWeaponChannelProperty = ECC_GameTraceChannel4;

	UFUNCTION(BlueprintCallable)
	void FireWeaponLineTrace();

	virtual	void FireWeaponCall_Implementation() override;

	virtual bool HasAmmo_Implementation() const override;

	virtual int32 GetAmmo_Implementation() const override;

	virtual void Reload_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 CurrentAmmo = 8;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Weapon")
	int32 MaxAmmo = 8;
	UPROPERTY(EditAnywhere,	BlueprintReadWrite, Category = "Weapon")
	bool bHasAmmo = true;
	UPROPERTY(EditAnywhere,	BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 25;
		
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


