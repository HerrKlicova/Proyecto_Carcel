// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box Component");
	RootComponent = BoxComponent;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	WeaponMesh->SetupAttachment(BoxComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Interact_Implementation()
{
	Destroy();
}

FItemData AWeapon::GetItemData_Implementation()
{
	return WeaponItemData;
}


void AWeapon::FireWeaponLineTrace()
{
	FHitResult Hit;
	
	FTransform SocketLocation = WeaponMesh->GetSocketTransform(FName("MuzzleFlash"), RTS_World);
	FRotator SocketRotation = SocketLocation.GetRotation().Rotator();
	FVector SocketForward = SocketRotation.Vector();
	
	FVector StartLocation = SocketLocation.GetLocation();

	FVector EndLocation = StartLocation + SocketForward * 600.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, FireWeaponChannelProperty, QueryParams);

	CurrentAmmo--;
	CurrentAmmo = FMath::Clamp(CurrentAmmo, 0, MaxAmmo);
	if (CurrentAmmo <= 0)
	{
		bHasAmmo = false;
	}
	DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.TraceEnd, Hit.bBlockingHit ? FColor::Cyan : FColor::Yellow, false, 0.3, 0, 1.0f);
}

void AWeapon::FireWeaponCall_Implementation()
{
	if (bHasAmmo)
	{
		FireWeaponLineTrace();
	}
}

bool AWeapon::HasAmmo_Implementation() const
{
	return bHasAmmo;
}

int32 AWeapon::GetAmmo_Implementation() const
{
	return CurrentAmmo;
}

void AWeapon::Reload_Implementation()
{
	if (CurrentAmmo == 0)
	{
		CurrentAmmo = MaxAmmo;
		bHasAmmo = true;
	}
}
