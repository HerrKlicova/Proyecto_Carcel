// Fill out your copyright notice in the Description page of Project Settings.


#include "ACTORS/INTERACTUABLE/InteractuableActor.h"

// Sets default values
AInteractuableActor::AInteractuableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = BoxComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(BoxComponent);
}

// Called when the game starts or when spawned
void AInteractuableActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractuableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractuableActor::Interact_Implementation()
{	
	Destroy();
}

FItemData AInteractuableActor::GetItemData_Implementation()
{
	FItemData ClassData;
	ClassData.ItemID = ItemID;
	ClassData.ItemName = ItemNames;
	ClassData.ItemType = ItemType;
	ClassData.ItemIcon = ItemIcon;
	ClassData.EffectValue = EffectValue;
	ClassData.UseSound = UseSound;
	ClassData.PickupSound = PickupSound;
	
	return ClassData;
}

//	Shows the item name on creen. Called on detection function
FText AInteractuableActor::GetInteractionText_Implementation()
{
	return ItemName;
}