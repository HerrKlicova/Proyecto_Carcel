// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INTERFACES/InteractuableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "PLAYER CHARACTER/COMPONENTS/ItemTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "InteractuableActor.generated.h"

UCLASS()
class PROYECTO_CARCEL_API AInteractuableActor : public AActor, public IInteractuableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractuableActor();

	//	Create a sound property for interactuable actors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcel|Interactuable Actor|Audio")
	USoundBase* UseSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcel|Interactuable Actor|Audio")
	USoundBase* PickupSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcel|Interactuable Actor|StaticMesh")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcel|Interactuable Actor|Collision")
	UBoxComponent* BoxComponent;
	
	//	Custom name for interactuable actors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcel|Interactuable Actor|Interact")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcel|Interactuable Actor|Item Type")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcel|Interactuable Actor|Item Type")
	FString ItemNames;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcel|Interactuable Actor|Item Type")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcel|Interactuable Actor|Item Type")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carcel|Interactuable Actor|Item Type")
	float EffectValue;
	
	//	Implement interaction interface function
	UFUNCTION(BlueprintCallable, Category = "Interact")
	virtual void Interact_Implementation() override;

	virtual FItemData GetItemData_Implementation() override;
	
	//	Implement Show interaction text function
	UFUNCTION(BlueprintCallable, Category = "Interact")
	virtual FText GetInteractionText_Implementation() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
