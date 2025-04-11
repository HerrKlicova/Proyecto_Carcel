// Fill out your copyright notice in the Description page of Project Settings.
#include "ACTORS/DAMAGGING/TrapActorC.h"
#include "INTERFACES/DamageableInterface.h"


// Sets default values
ATrapActorC::ATrapActorC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//	create component
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("TrapCollision");
	BoxCollision->SetupAttachment(RootComponent);
	//	on begin overlap, call the TrapBeginOverlap function on this class
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapActorC::TrapBeginOverlap);
}



// Called when the game starts or when spawned
void ATrapActorC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrapActorC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//	called from begin overlap, checks if implements interface UDamageanbleInterface and executes ApplyDamage function...
//	...created on the interface
void ATrapActorC::TrapBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->Implements<UDamageableInterface>())
	{
		IDamageableInterface::Execute_ApplyDamage(OtherActor, DamageAmount);
	}
}